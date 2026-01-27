// Fill out your copyright notice in the Description page of Project Settings.


#include "Item.h"
#include "Gosoo/UI/ObjectUI/InteractUI.h"
#include "Components/StaticMeshComponent.h"
#include "Components/WidgetComponent.h"
#include "Gosoo/UI/ObjectUI/ConversationUI.h"


AItem::AItem()
{
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	MeshComponent->SetupAttachment(RootComponent);

	TextWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("TextWidgetComponent"));
	TextWidgetComponent->SetupAttachment(RootComponent);
	TextWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
	TextWidgetComponent->SetDrawSize(FVector2D(1920.0f, 1080.0f));

	static ConstructorHelpers::FClassFinder<UUserWidget>TempWidget(TEXT("/Game/0_DEV/KYS/UI/InteractUI/WBP_InteractUI.WBP_InteractUI_C"));
	if (TempWidget.Succeeded())
	{
		TextWidgetComponent->SetWidgetClass(TempWidget.Class);
	}
}


void AItem::BeginPlay()
{
	Super::BeginPlay();


	if (TextWidgetComponent)
	{
		TextWidgetComponent->SetVisibility(false);    // 시작할땐 위젯이 안보이게 하고 시작

		if (UInteractUI* InteractUIObj = Cast<UInteractUI>(TextWidgetComponent->GetWidget()))
		{
			InteractUIObj->SetInteractableObjectText(InteractText);  // 위젯에 띄울 문구는 내가 변수로 저장해놓은 텍스트로 변경.

			GEngine->AddOnScreenDebugMessage(-1, 4.0f, FColor::Blue, TEXT("SetInteractText"));
		}
	}

	// 단순히 아이템 옆에서 상호작용 가능하다는 글만 띄울것이므로 중요한 것이 아니다.
	// 따라서 서버에서만 판단 할 이유가 없어서 이렇게 로컬에서 실행하도록 한다.
	RangeBox->OnComponentBeginOverlap.AddDynamic(this, &AItem::RangeBoxBeginOverlap);
	RangeBox->OnComponentEndOverlap.AddDynamic(this, &AItem::RangeBoxEndOverlap);
}

void AItem::RangeBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (AGosooPlayer* OverlapPlayer = Cast<AGosooPlayer>(OtherActor))
	{
		if (OverlapPlayer->IsLocallyControlled())
		{
			InteractPlayer = OverlapPlayer;

			InteractPlayer->SetInteractableActor(this);

			TextWidgetComponent->SetVisibility(true);
		}
	}
}

void AItem::RangeBoxEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (InteractPlayer == Cast<AGosooPlayer>(OtherActor))
	{

		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Emerald, TEXT("TEXT NICE"));

		InteractPlayer->SetInteractableActor(nullptr);

		InteractPlayer = nullptr;

		TextWidgetComponent->SetVisibility(false);
	}
}

void AItem::OpenInteractInputMode()
{
	if (InteractPlayer)
	{
		if (APlayerController* InteractController = Cast<APlayerController>(InteractPlayer->GetController()))
		{
			InteractController->bShowMouseCursor = true;  // 마우스 커서 보이기

			ConversationUIObject->bIsFocusable = true;

			FInputModeUIOnly InputMode;      
			
			InteractController->FlushPressedKeys();

			InputMode.SetWidgetToFocus(ConversationUIObject->TakeWidget());   // 입력 모드를 UI 중심으로 변경
			InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);

			InteractController->SetInputMode(InputMode);
		}
	}
}

void AItem::CloseInteractInputMode()
{
	if (ConversationUIObject)
	{
		ConversationUIObject->bIsFocusable = false;

		ConversationUIObject->RemoveFromViewport();
	}

	if (InteractPlayer)
	{
		if (APlayerController* InteractController = Cast<APlayerController>(InteractPlayer->GetController()))
		{
			InteractController->bShowMouseCursor = false;  // 마우스 커서 보이기

			FInputModeGameOnly InputMode;

			InteractController->SetInputMode(InputMode);
		}
	}
}

void AItem::InteractInterfaceFunction_Implementation(AGosooPlayer* PlayerCharacter)
{
	if (ConversationUIClass)
	{
		if (!ConversationUIObject)
		{
			ConversationUIObject = CreateWidget<UConversationUI>(GetWorld(), ConversationUIClass);

			ConversationUIObject->SetCopyConversationTexts(ConversationTexts);   // 이 오브젝트가 가지고 있는 대화를 넘김.

			ConversationUIObject->AddToViewport();

			ConversationUIObject->SetOwnerInteractObject(this);

			OpenInteractInputMode();
		}
		else
		{
			ConversationUIObject->AddToViewport();

			OpenInteractInputMode();
		}
	}
}




