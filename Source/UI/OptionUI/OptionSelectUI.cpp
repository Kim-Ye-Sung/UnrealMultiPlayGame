// Fill out your copyright notice in the Description page of Project Settings.


#include "OptionSelectUI.h"
#include "Components/Button.h"
#include "Gosoo/Player/GosooPlayer.h"
#include "Components/TextBlock.h"

void UOptionSelectUI::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	SoundOptionButton->OnClicked.AddDynamic(this, &UOptionSelectUI::SoundOptionWidgetOpen);
	SoundOptionButton->OnHovered.AddDynamic(this, &UOptionSelectUI::SetSoundTextColor);
	SoundOptionButton->OnUnhovered.AddDynamic(this, &UOptionSelectUI::SetSoundTextColor);

	DisplayOptionButton->OnClicked.AddDynamic(this, &UOptionSelectUI::DisplayOptionWidgetOpen);
	DisplayOptionButton->OnHovered.AddDynamic(this, &UOptionSelectUI::SetDisplayTextColor);
	DisplayOptionButton->OnUnhovered.AddDynamic(this, &UOptionSelectUI::SetDisplayTextColor);

	KeySettingOptionButton->OnClicked.AddDynamic(this, &UOptionSelectUI::KeySettingOptionWidgetOpen);
	KeySettingOptionButton->OnHovered.AddDynamic(this, &UOptionSelectUI::SetKeySettingTextColor);
	KeySettingOptionButton->OnUnhovered.AddDynamic(this, &UOptionSelectUI::SetKeySettingTextColor);

	MouseSettingOptionButton->OnClicked.AddDynamic(this, &UOptionSelectUI::MouseSettingOptionWidgetOpen);
	MouseSettingOptionButton->OnHovered.AddDynamic(this, &UOptionSelectUI::SetMouseSettingTextColor);
	MouseSettingOptionButton->OnUnhovered.AddDynamic(this, &UOptionSelectUI::SetMouseSettingTextColor);

	//if (SoundOptionWidgetClass)   // 사운드 위젯 클래스가 있는지 확인
	//{
	//	SoundOptionWidgetObj = CreateWidget<UUserWidget>(GetWorld(), SoundOptionWidgetClass);   // 사운드 옵션창 생성

	//	CurrentOptionWidget = SoundOptionWidgetObj;    // 현재 창을 사운드 옵션 창으로 설정 후 

	//	CurrentOptionText = SoundText;
	//	
	//	CurrentOptionText->SetColorAndOpacity(FSlateColor(FLinearColor::Red));

	//	SoundOptionWidgetObj->AddToViewport(1);     // 사운드 옵션 창 열기
	//}
}

// --------------------------------------
// 옵션 창을 열면 제일 처음은 사운드 창이 열린다 치더라도
// 옵션창 껐다가 다시 열어도 사운드 창 시작으로 할건지 문의
// 사운드창으로 시작한다고 하면 밑의 함수를 그대로 적용하고 아니라면 수정하기.
// --------------------------------------

// --------------------------------------
// 호버드 언호버드 할건지 기획서 보고 수정해야할게 있음
// 옵션텍스트중 어느것이라도 호버드한 상태로 옵션창을 끄고, 
// 다시 옵션창을 켜면 마우스가 어느 위치에 있던간에 마지막으로 호버드한 텍스트가 그대로 빨간색임.
// 끌때는 언호버드 처리가 안되서 이러는거같은데, 호버드, 언호버드 할건지에 따라 수정하기.
// --------------------------------------


void UOptionSelectUI::NativeConstruct()
{
	Super::NativeConstruct();

	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Blue, TEXT("Construct On!!!"));

	OpenOptionInputMode();

	// 항상 옵션창 열때 시작은 사운드 옵션으로 시작하기 위해 아래와 같이 코드 작성.
	if (SoundOptionWidgetObj)        
	{
		CurrentOptionWidget = SoundOptionWidgetObj;

		CurrentOptionWidget->AddToViewport(1);

		SetCurrentOptionText(SoundText);

		CurrentOptionText->SetColorAndOpacity(FSlateColor(FLinearColor::Red));
	}
	else
	{
		if (SoundOptionWidgetClass)   // 사운드 위젯 클래스가 있는지 확인
		{
			SoundOptionWidgetObj = CreateWidget<UUserWidget>(GetWorld(), SoundOptionWidgetClass);   // 사운드 옵션창 생성

			CurrentOptionWidget = SoundOptionWidgetObj;    // 현재 창을 사운드 옵션 창으로 설정 후 

			CurrentOptionText = SoundText;

			CurrentOptionText->SetColorAndOpacity(FSlateColor(FLinearColor::Red));

			CurrentOptionWidget->AddToViewport(1);     // 사운드 옵션 창 열기
		}
	}

	//SoundOptionWidgetOpen();   // Add To Viewport 할때마다 사운드창으로 시작하게 하기
}

FReply UOptionSelectUI::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	if (InKeyEvent.GetKey() == EKeys::C) 
	{
		CloseOptionInputMode();   
		return FReply::Handled();
	}

	//return FReply::Unhandled();
	return Super::NativeOnKeyDown(InGeometry, InKeyEvent);
}

void UOptionSelectUI::SoundOptionWidgetOpen()
{	
	if (CurrentOptionWidget == SoundOptionWidgetObj)     // 현재 옵션 창이 사운드 옵션창이면 버튼을 계속 눌러도 그냥 무시
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Blue, TEXT("Sound Return On!!!"));

		return;
	}

	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Blue, TEXT("Sound On!!!"));

	SetCurrentOptionWidget(SoundOptionWidgetObj);      // 다른 옵션 창 끄고 사운드 옵션창을 켜기

	SetCurrentOptionText(SoundText);
}

void UOptionSelectUI::DisplayOptionWidgetOpen()
{
	if (CurrentOptionWidget == DisplayOptionWidgetObj)     // 현재 옵션 창이 디스플레이 옵션창이면 버튼을 계속 눌러도 그냥 무시
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Blue, TEXT("Display return On!!!"));

		return;
	}

	if (DisplayOptionWidgetObj)      // 디스플레이 옵션창이 이미 있으면
	{
		SetCurrentOptionWidget(DisplayOptionWidgetObj);  // 다른 옵션 창 끄고 디스플레이 옵션창 켜ㅣ

		SetCurrentOptionText(DisplayText);     // 디스플레이 텍스트 색깔 빨간색으로 변경
	}
	else       // 디스플레이 옵션창이 없으면                       
	{
		if (DisplayOptionWidgetClass)   // 디스플레이 위젯 클래스가 있는지 확인
		{
			DisplayOptionWidgetObj = CreateWidget<UUserWidget>(GetWorld(), DisplayOptionWidgetClass);   // 디스플레이 옵션창 생성

			SetCurrentOptionWidget(DisplayOptionWidgetObj);  // 다른 옵션 창 끄고 디스플레이창을 켜기

			SetCurrentOptionText(DisplayText);     // 디스플레이 텍스트 색깔 빨간색으로 변경
		}

	}
}

void UOptionSelectUI::KeySettingOptionWidgetOpen()
{
	if (CurrentOptionWidget == KeySettingOptionWidgetObj)     // 현재 옵션 창이 키세팅 옵션창이면 버튼을 계속 눌러도 그냥 무시
	{
		return;
	}

	if (KeySettingOptionWidgetObj)      // 키세팅 옵션창이 이미 있으면
	{
		SetCurrentOptionWidget(KeySettingOptionWidgetObj);   // 다른 옵션창 끄고 키세팅 옵션창 켜기

		SetCurrentOptionText(KeySettingText);     // 키세팅 텍스트 색깔 빨간색으로 변경
	}
	else       // 키 세팅 옵션창이 없으면                       
	{
		if (KeySettingOptionWidgetClass)   // 키세팅 위젯 클래스가 있는지 확인
		{
			KeySettingOptionWidgetObj = CreateWidget<UUserWidget>(GetWorld(), KeySettingOptionWidgetClass);   // 키세팅 옵션창 생성

			SetCurrentOptionWidget(KeySettingOptionWidgetObj);    // 다른 옵션창 끄고 키세팅 옵션창 켜기

			SetCurrentOptionText(KeySettingText);     // 키세팅 텍스트 색깔 빨간색으로 변경
		}

	}
}

void UOptionSelectUI::MouseSettingOptionWidgetOpen()
{
	if (CurrentOptionWidget == MouseSettingOptionWidgetObj)     // 현재 옵션 창이 마우스 세팅 옵션창이면 버튼을 계속 눌러도 그냥 무시
	{
		return;
	}

	if (MouseSettingOptionWidgetObj)      // 마우스 세팅 옵션창이 이미 있으면
	{
		SetCurrentOptionWidget(MouseSettingOptionWidgetObj);   // 다른 옵션창 끄고 마우스세팅 옵션창 켜기

		SetCurrentOptionText(MouseSettingText);     // 마우스세팅 텍스트 색깔 빨간색으로 변경
	}
	else       // 마우스 세팅 옵션창이 없으면                       
	{
		if (MouseSettingOptionWidgetClass)   // 마우스 세팅 위젯 클래스가 있는지 확인
		{
			MouseSettingOptionWidgetObj = CreateWidget<UUserWidget>(GetWorld(), MouseSettingOptionWidgetClass);   // 마우스 세팅 옵션창 생성

			SetCurrentOptionWidget(MouseSettingOptionWidgetObj);  // 다른 옵션창 끄고 마우스세팅 옵션창 켜기

			SetCurrentOptionText(MouseSettingText);     // 마우스세팅 텍스트 색깔 빨간색으로 변경
		}

	}
}

void UOptionSelectUI::SetCurrentOptionWidget(UUserWidget* CurrentOptionObj)
{
	CurrentOptionWidget->RemoveFromParent();     // 다른 옵션창을 닫고

	CurrentOptionWidget = CurrentOptionObj;  // 현재 창을 사운드 옵션 창으로 설정 후

	CurrentOptionWidget->AddToViewport();       // 현재 옵션 창 열기

	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Black, TEXT("WidgetOptionOpen"));
}

void UOptionSelectUI::OpenOptionInputMode()
{
	if (APlayerController* OwnerController = Cast<APlayerController>(OwnerPlayer->GetController()))
	{
		OwnerController->FlushPressedKeys();    // 키 입력받고 있던 것들 전부 지우기.

		OwnerController->bShowMouseCursor = true;  // 마우스 커서 보이기

		this->bIsFocusable = true;   // UserWidget이 기본적으로 가지고 있는 bIsFocusable이라는 변수를 true로 해줘야 
									 // 밑에 코드에서 SetWidgetToFocus로 이 UI에 집중하여 마우스랑 키보드 값을 받을수있다.

		FInputModeUIOnly ControllerInputMode;     // UI만 클릭할 수 있는 인풋모드 생성

		ControllerInputMode.SetWidgetToFocus(this->TakeWidget());
		ControllerInputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);

		OwnerController->SetInputMode(ControllerInputMode);
	}
}

void UOptionSelectUI::CloseOptionInputMode()
{
	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Black, TEXT("CloseOption"));

	if (APlayerController* OwnerController = Cast<APlayerController>(OwnerPlayer->GetController()))
	{
		OwnerController->bShowMouseCursor = false;  // 마우스 커서 보이기

		this->bIsFocusable = false;

		FInputModeGameOnly ControllerInputMode;     // UI만 클릭할 수 있는 인풋모드 생성
		OwnerController->SetInputMode(ControllerInputMode);

		CurrentOptionWidget->RemoveFromParent();

		if (IsMouseHovered)   // 옵션창 끌때, 호버드 된 버튼이 있었다면 
		{
			HoveredText->SetColorAndOpacity(FSlateColor(FLinearColor::White)); //버튼의 텍스트를 흰색으로 변경
		}

		this->RemoveFromParent();
		
	}
}

void UOptionSelectUI::SetSoundTextColor()
{
	if (CurrentOptionWidget == SoundOptionWidgetObj)
	{
		return;
	}

	IsMouseHovered = !IsMouseHovered;   // hovered일때는 true로 변경하고, unhovered일때는 false로 변경.

	HoveredText = SoundText;

	HoveredTextColorChange();
}

void UOptionSelectUI::SetDisplayTextColor()
{
	if (CurrentOptionWidget == DisplayOptionWidgetObj)
	{
		return;
	}

	IsMouseHovered = !IsMouseHovered;

	HoveredText = DisplayText;

	HoveredTextColorChange();
}	

void UOptionSelectUI::SetKeySettingTextColor()
{
	if (CurrentOptionWidget == KeySettingOptionWidgetObj)
	{
		return;
	}

	IsMouseHovered = !IsMouseHovered;

	HoveredText = KeySettingText;

	HoveredTextColorChange();
}

void UOptionSelectUI::SetMouseSettingTextColor()
{
	if (CurrentOptionWidget == MouseSettingOptionWidgetObj)
	{
		return;
	}

	IsMouseHovered = !IsMouseHovered;

	HoveredText = MouseSettingText;

	HoveredTextColorChange();
}

void UOptionSelectUI::HoveredTextColorChange()
{
	if (IsMouseHovered)
	{
		HoveredText->SetColorAndOpacity(FSlateColor(FLinearColor::Red));
	}
	else
	{
		HoveredText->SetColorAndOpacity(FSlateColor(FLinearColor::White));
	}
}

void UOptionSelectUI::SetCurrentOptionText(UTextBlock* OptionText)
{
	CurrentOptionText->SetColorAndOpacity(FSlateColor(FLinearColor::White));     // 이전 옵션텍스트는 하얀색으로 변경

	CurrentOptionText = OptionText;       // 현재 옵션 텍스트를 새롭게 누른 옵션텍스트로 변경

	//CurrentOptionText->SetColorAndOpacity(FSlateColor(FLinearColor::Red));   // 현재 옵션 텍스트를 빨간색으로 변경

	IsMouseHovered = false;  // clicked가 실행된 이후로는 
							 // 위쪽의 텍스트컬러를 변경하는 함수들에서 
							 // CurrentOptionWidget == 의 조건문에 의해
							 // return하게 되어 사실상 unhovered의 함수가 실행되지 않는것처럼 된다.
							 // 따라서 여기서 직접 이 변수를 변경해준다.
}
