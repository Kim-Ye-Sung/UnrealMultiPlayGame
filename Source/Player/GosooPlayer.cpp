// Fill out your copyright notice in the Description page of Project Settings.

// Project_Gosoo에서 사용할 캐릭터 클래스의 cpp 파일.

#include "GosooPlayer.h"
#include "Camera/CameraComponent.h"    // 카메라 컴포넌트를 사용하기 위한 헤더파일 추가
#include "Components/CapsuleComponent.h"   // 캐릭터의 캡슐컴포넌트를 사용하기 위한 헤더파일 추가
										   // (기본 캐릭터 클래스가 캡슐컴포넌트가 루트 컴포넌트로 되어 있으니, 
										   //  카메라와 스프링암을 루트 컴포넌트에 붙여주기 위해 사용하는 것.)
#include "GameFramework/CharacterMovementComponent.h"  // 캐릭터무브먼트 컴포넌트를 사용하기 위한 헤더파일 추가
#include "GameFramework/SpringArmComponent.h"    // 스프링암 컴포넌트를 사용하기 위한 헤더파일 추가
#include "Components/InputComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Net/UnrealNetwork.h"   // GetLifetimeReplicatedProps함수를 사용하여 복제가능한 변수를 설정하기 위한 헤더파일 추가
#include "Components/ArrowComponent.h"   // 기본 캐릭터 클래스의 애로우 컴포넌트를 사용하기 위한 헤더파일 추가
#include "PlayerStat.h"
#include "Gosoo/InteractableObject/InteractInterface.h"      // 상호작용을 위한 InteractInterface 헤더파일 추가 
#include "Kismet/KismetSystemLibrary.h"   // 라인트레이스 사용을 위한 헤더파일 추가
#include "PlayerSkill.h"
#include "Components/CapsuleComponent.h"
#include"../Enemy/Drone_Enemy.h"
#include "Engine/DamageEvents.h"
#include "Gosoo/UI/PlayerUI/PlayerUI.h"
#include "Gosoo/UI/PlayerUI/MapUI.h"
#include "Components/SceneCaptureComponent2D.h"   // 씬캡쳐2D 컴포넌트를 위한 헤더파일 추가 
#include "Engine/TextureRenderTarget2D.h"    // 미니맵의 텍스쳐를 생성하기 위한 헤더파일 추가
#include "Components/BoxComponent.h"
#include "Gosoo/UI/OptionUI/OptionSelectUI.h"

// Sets default values
AGosooPlayer::AGosooPlayer()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PlayerSpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("PlayerSpringArmComponent"));
	PlayerSpringArmComponent->SetupAttachment(RootComponent);      // 루트컴포넌트(==캡슐 컴포넌트)의 자식 컴포넌트로 붙임.
	PlayerSpringArmComponent->TargetArmLength = 400.0f;       // TargetArmLength를 400으로 설정.  *********************(이것은 상의 후 변경할 것.)***********************  
	PlayerSpringArmComponent->bUsePawnControlRotation = true;    // 스프링암이 Pawn의 컨트롤 회전을 따라 회전하게끔 설정.


	PlayerCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("PlayerCameraComponent"));
	PlayerCameraComponent->SetupAttachment(PlayerSpringArmComponent, USpringArmComponent::SocketName);   // 스프링암의 끝부분에 있는 소켓에 카메라 컴포넌트를 위치시킴.
	PlayerCameraComponent->bUsePawnControlRotation = false;   // 스프링암에서 true로 이미 회전을 처리하니 자식으로 붙은 여기선 false로 설정. 즉, 스프링암으로만 카메라를 움직인다.


	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;
	// 이 3개를 false로 해줌으로서 카메라를 회전해도 캐릭터가 회전하지 않고 가만히 있고, W,A,S,D처럼 움직임 입력을 받았을때만 회전한다. 
	// 즉, 우리가 구현하고자 하는 가만히 있을때 캐릭터 구경하다가 움직일때만 다시 정면을 바라보고 움직이는게 가능해진다.

	GetCapsuleComponent()->InitCapsuleSize(60.0f, 96.f);   // 캐릭터의 캡슐 컴포넌트 크기 변경

	GetMesh()->SetRelativeLocation(FVector(0.0f, 0.0f, -100.0f));
	GetMesh()->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
	static ConstructorHelpers::FObjectFinder<USkeletalMesh>PlayerMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/01_ReSource/CyberpunkPackVol1/Characters/Raven/Meshes/SK_Raven_FullBody_02.SK_Raven_FullBody_02'"));
	if (PlayerMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(PlayerMesh.Object);
	}

	GetCharacterMovement()->MaxWalkSpeed = 350.0f;
	GetCharacterMovement()->bOrientRotationToMovement = true;     // 캐릭터가 이동하는 방향을 자동으로 바라보게 하는 설정. false로하면 항상 현재 방향을 바라본 채로 움직임.
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);    // 캐릭터가 이동하는 방향으로 바라볼때, 캐릭터 몸체가 회전하는 속도.          
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.0f;   // 걷다가 멈출때 얼마나 빨리 멈추는지를 설정. 값이 클수록 빠르게 정지한다.

	JumpMaxCount = 2;   // 더블 점프가 가능하게끔 기본캐릭터 클래스에 있는 변수인 JumpMaxCount를 2로 설정.

	CharacterStateComponent = CreateDefaultSubobject<UCharacterState>(TEXT("PlayerState"));    // 캐릭터 Enum 생성.

	Stat = CreateDefaultSubobject<UPlayerStat>(TEXT("Stat"));    // 캐릭터 스탯 생성

	//SkillComponent = CreateDefaultSubobject<UPlayerSkill>(TEXT("SkillComponent"));   // 캐릭터 스킬 컴포넌트 생성
	// ---------------------------------------------------------
	// 스킬 컴포넌트같은 경우는 캐릭터마다 스킬이 다르므로 스킬 컴포넌트의 자식을 자식 캐릭터 클래스에서 바꿔줄 생각이다.
	// 그런데 위와 같이 자식 클래스에서도 SkillComponent = CreateDefaultSubobject<UEvelynnSkill>(TEXT("SkillComponent"));
	// 이런식으로 해버리면 내용물이 제대로 보이지 않는다. 
	// 아마도 이런식으로 해버리면 이미 부모에서도 생성했는데, 자식도 생성해서 다르게 붙여주는거라 이상하게 작동하는듯 하다.


	//ServerComponent = CreateDefaultSubobject<UServerComponent>(TEXT("ServerComponent"));


	MeleeAttackHitBox = CreateDefaultSubobject<UBoxComponent>(TEXT("MeleeAttackHitBox"));
	MeleeAttackHitBox->SetupAttachment(GetMesh(), TEXT("hand_l"));    // 근접공격 판정 히트 박스를 메시의 왼손에 붙임.
	// 생성자에서 붙일때는 SetupAttachment를 사용하고, 이후 다른 곳에서 런타임중에 붙일때는 AttachToComponent를 사용한다.

	MeleeAttackHitBox->bHiddenInGame = false;    // 박스컴포넌트가 보이게끔 설정
	MeleeAttackHitBox->SetBoxExtent(FVector(30.0f, 30.0f, 30.0f));
	MeleeAttackHitBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);  // 충돌 무시하기.

	// ------------------------------ 미니맵 관련 설정 -------------------------------------------//
	MinimapSpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("MinimapSpringArmComponent"));
	MinimapSpringArmComponent->SetupAttachment(RootComponent);
	MinimapSpringArmComponent->bInheritYaw = false;
	MinimapSpringArmComponent->bInheritPitch = false;
	MinimapSpringArmComponent->bInheritRoll = false;
	MinimapSpringArmComponent->TargetArmLength = 500.0f;
	MinimapSpringArmComponent->SetRelativeRotation(FRotator(-90.0f, 0.0f, 0.0f));

	MinimapCameraComponent = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("MinimapCameraComponent"));
	MinimapCameraComponent->SetupAttachment(MinimapSpringArmComponent);
	MinimapCameraComponent->ProjectionType = ECameraProjectionMode::Orthographic;  // 정사영법으로 투영
	MinimapCameraComponent->OrthoWidth = 1700.0f;    // 맵범위를 얼마나 보일것인가를 결정. 숫자가 클수록 맵이 위에서 넓게 보이고, 캐릭터는 작아짐.
}

// Called when the game starts or when spawned
void AGosooPlayer::BeginPlay()
{
	Super::BeginPlay();

	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

	PlayerAnimInstance = GetMesh()->GetAnimInstance(); // 서버쪽에서 모든 캐릭터의 애님인스턴스를 가져와서 복제해주면 될것 같지만 그러면 안된다.
	// 애님인스턴스는 로컬렌더링 객체이므로 서버가 애님인스턴스를 복제해준다고 해도 서버에서의 애님인스턴스 주소값과
	// 클라이언트에서의 애님인스턴스 주소값이 다르기 때문이다. 따라서 이렇게 로컬로 각자 모든 캐릭터 인스턴스의 애님인스턴스를 가져와야한다.

	if (HasAuthority())
	{
		SetWeapon2();

		MeleeAttackHitBox->OnComponentBeginOverlap.AddDynamic(this, &AGosooPlayer::MeleeHitBoxBeginOverlap);
		MeleeAttackHitBox->OnComponentEndOverlap.AddDynamic(this, &AGosooPlayer::MeleeHitBoxEndOverlap);
	}


	if (IsLocallyControlled())  // 자기 자신만의 캐릭터 UI를 띄운다. 
		                        // 만약 이 조건이 없으면 각 서버와 클라이언트에서 자기자신과 더불어 다른 클라이언트들의 UI도 생성하고 
		                        // 델리게이트에 바인딩하므로 서로 UI가 섞이는 이상한 현상이 발생한다.
		                        // 실제로 이 조건없이 돌리면 클라이언트2의 체력이 줄어들어면 클라이언트1 또는 서버의 UI 체력바가 변경된다.
	{


		MinimapRenderTarget = NewObject<UTextureRenderTarget2D>(this);  // 미니맵을 찍을 텍스쳐를 동적으로 생성 
		// 진짜 주의해야할게 콘텐트 브라우저상에 텍스쳐 하나 미리 생성해두고 MinimapCameraComponent의 TextureTarget에 할당해줬더니
		// 서버와 클라이언트1, 클라이언트2가 전부 클라이언트1의 미니맵을 공유함. 그래서 서버 캐릭터와 클라이언트2가 움직여도 자기들 미니맵은 정지되어있고,
		// 클라이언트1이 움직이면 모든 미니맵이 클라이언트1의 움직임에 맞춰서 움직였다.
		// 이 경험을 토대로 알아보니 텍스쳐는 각 캐릭터 인스턴스별로 따로 생성되는게 아니고, 콘텐트 브라우저에 있는 그 텍스쳐가 덮어씌워지는 방식인 것이라는 것을 알아냈다.
		// 따라서 각 캐릭터별로 다른 미니맵을 찍으려면 미리 여러개의 텍스쳐를 생성해서 따로 할당해주던가 아니면 이렇게 동적으로 생성해줘야한다.

		MinimapRenderTarget->InitAutoFormat(256, 256);  // 텍스쳐 사이즈 설정.

		if (MinimapCameraComponent != nullptr && MinimapRenderTarget != nullptr)   // 미니맵 관련 컴포넌트와 텍스쳐가 제대로 생성이 됐다면
		{
			MinimapCameraComponent->TextureTarget = MinimapRenderTarget;   // 컴포넌트에 생성된 텍스쳐를 할당.
		}

		if (PersonalPlayerUIClass != nullptr)
		{
			PersonalPlayerUI = CreateWidget<UPlayerUI>(GetWorld(), PersonalPlayerUIClass);

			if (PersonalPlayerUI != nullptr)
			{
				PersonalPlayerUI->AddToViewport();   // 플레이어 UI 화면 켜고

				Stat->HP_Change_Function.AddDynamic(PersonalPlayerUI, &UPlayerUI::Set_HP_Text); // UI에서 체력 숫자 표기 바꾸는 함수 델리게이트바인딩 하기
				Stat->HP_Change_Function.Broadcast(Stat->HP, Stat->MaxHP);

				if (SkillComponent)
				{
					SkillComponent->Skill_One_Change.AddDynamic(PersonalPlayerUI, &UPlayerUI::SetFirstSkillProgress);
					SkillComponent->Skill_One_Change.Broadcast(SkillComponent->Skill_One_RemainingTime, SkillComponent->Skill_One_Cooldown);
					SkillComponent->Skill_Two_Change.AddDynamic(PersonalPlayerUI, &UPlayerUI::SetSecondSkillProgress);
					SkillComponent->Skill_Two_Change.Broadcast(SkillComponent->Skill_Two_RemainingTime, SkillComponent->Skill_Two_Cooldown);
				}

				if (MinimapRenderTarget != nullptr)  
				{
					PersonalPlayerUI->SetMinimapImage(MinimapRenderTarget);  // 미니맵 띄우기 기능 실행.
				}

			}
		}
		
	}
}

// Called every frame
void AGosooPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	if (GetLocalRole() != ROLE_Authority)  // 서버가 아니라면 실행하지 않음.  되도록이면 게임의 공정성이 판단되는 요소들은 전부 서버에서만 하게 해둘것. 
	{
		return;
	}

	//FHitResult HitResult;
	//FCollisionQueryParams QueryParams;
	//QueryParams.bTraceComplex = true;
	//QueryParams.AddIgnoredActor(this);

	//auto SphereRadius = 50.0f;
	//auto StartLocation = GetActorLocation() + GetActorForwardVector() * 150.0f;
	//auto EndLocation = StartLocation + GetActorForwardVector() * 1000.0f;

	//auto IsHit = UKismetSystemLibrary::SphereTraceSingle(
	//	GetWorld(),
	//	StartLocation,
	//	EndLocation,
	//	SphereRadius,
	//	UEngineTypes::ConvertToTraceType(ECC_WorldDynamic),
	//	false,
	//	TArray<AActor*>(),
	//	EDrawDebugTrace::ForOneFrame,
	//	HitResult,
	//	true
	//);

	//if (IsHit && HitResult.GetActor()->GetClass()->ImplementsInterface(UInteractable::StaticClass()))
	//{
	//	DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, SphereRadius, 12, FColor::Magenta, false, 1.0f);

	//	InterActor = HitResult.GetActor();
	//}
	//else
	//{
	//	InterActor = nullptr;
	//}



	//FVector StartPos = PlayerCameraComponent->GetComponentLocation();    // 라인트레이스 시작 위치
	//FVector EndPos = StartPos + PlayerCameraComponent->GetForwardVector() * 5000.0f;    // 라인트레이스 종료 위치
	//FCollisionQueryParams Params;    // 여러가지 충돌 옵션을 설정할 수 있는 구조체 선언
	//Params.AddIgnoredActor(this);    // 여러가지 설정중에서 이것을 이용하여 자기자신은 라인트레이스 대상에서 무시하게끔 설정.
	//bHit = GetWorld()->LineTraceSingleByChannel(HitInfo, StartPos, EndPos, ECC_Visibility, Params);


	 


	IsLanding = !(GetCharacterMovement()->IsFalling());   // 서버쪽에서 모든 캐릭터 인스턴스가 현재 공중에 있는지 여부를 확인하여 공중에 있지 않으면 지상에 있는 상태로 설정

	if (CharacterStateComponent->GetCurrentState() == EPlayerState::Jump && IsLanding)  // 캐릭터가 Jump상태이면서 지상에 있다고 한다면  == 착지했다면
	{
		CharacterStateComponent->SetCurrentState(EPlayerState::Move);

		JumpCount = 0;   // 점프 횟수 초기화

		DoubleJumpEnd = false; // 다시 더블점프할 수 있게끔 변경
	}
}

// Called to bind functionality to input
void AGosooPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))   // 캐릭터가 사용할 키와 함수 바인딩
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AGosooPlayer::Basic_Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AGosooPlayer::Basic_Look);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &AGosooPlayer::Basic_Jump);
		EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Started, this, &AGosooPlayer::Basic_Crouch);
		EnhancedInputComponent->BindAction(Dash_Run_Action, ETriggerEvent::Started, this, &AGosooPlayer::Basic_Dash);
		EnhancedInputComponent->BindAction(Interaction, ETriggerEvent::Started, this, &AGosooPlayer::Basic_Interaction_Trace);
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Started, this, &AGosooPlayer::Basic_Attack);
		EnhancedInputComponent->BindAction<UPlayerSkill>(Skill_One, ETriggerEvent::Started, SkillComponent, &UPlayerSkill::Skill_1);
		EnhancedInputComponent->BindAction<UPlayerSkill>(Skill_Two, ETriggerEvent::Started, SkillComponent, &UPlayerSkill::Skill_2);
		EnhancedInputComponent->BindAction(MeleeAttack, ETriggerEvent::Started, this, &AGosooPlayer::Basic_MeleeAttack);
		EnhancedInputComponent->BindAction(EmotionAction, ETriggerEvent::Started, this, &AGosooPlayer::EmotionControl);
		EnhancedInputComponent->BindAction(Emotion1_SelectAction, ETriggerEvent::Started, this, &AGosooPlayer::Emotion1);
		EnhancedInputComponent->BindAction(Emotion2_SelectAction, ETriggerEvent::Started, this, &AGosooPlayer::Emotion2);
		EnhancedInputComponent->BindAction(Emotion3_SelectAction, ETriggerEvent::Started, this, &AGosooPlayer::Emotion3);
		EnhancedInputComponent->BindAction(MapAction, ETriggerEvent::Started, this, &AGosooPlayer::MapFunction);
		EnhancedInputComponent->BindAction(OptionOpenAction, ETriggerEvent::Started, this, &AGosooPlayer::OptionSelectOpen);

		EnhancedInputComponent->BindAction(TestInput, ETriggerEvent::Started, this, &AGosooPlayer::TestAction);
	}
}


void AGosooPlayer::Basic_Move(const FInputActionValue& Value)  // 플레이어 캐릭터의 이동을 처리하는 함수
{
	const auto MovementVector = Value.Get<FVector2D>(); // Value에는 사용자가 입력한 방향 정보가 들어있다.

	if (Controller != nullptr) // 캐릭터가 조종 가능한 상태일 때만 이동 처리 (Controller가 없으면 이동할 수 없음)
	{


		const auto Rotation = Controller->GetControlRotation();  // 플레이어가 보고 있는 방향(카메라 방향)의 회전값을 가져온다.
		const FRotator YawRotation(0, Rotation.Yaw, 0);	 // 위 Pitch(상하), Roll(기울기)는 무시하고 Yaw(좌우 방향)만 사용해서 회전값 생성


		const auto ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X); 	// YawRotation(화면이 보는 방향)을 기준으로 "앞 방향" 벡터를 구한다.
		const auto RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);  		// 같은 방식으로 "오른쪽 방향" 벡터도 구한다.

		AddMovementInput(ForwardDirection, MovementVector.X);		// 사용자가 입력한 X값(좌/우 방향)에 따라 앞/뒤로 이동
		//  (단, 에디터의 MappingContext의 W,S가 X축으로 설정되어있어야한다.)

		// ******  AddMovementInput의 장점  ******
		// AddMovementInput(Direction, Scale)  이렇게 함수에 방향과 크기를 인자로 넣어주면
		// CharacterMovementComponent에서 알아서 CurrentVelocity += Direction * Scale * MoveSpeed * DeltaTime 이렇게 계산한다.
		// 따라서 여기서는 DeltaTime을 고려하지 않아도 알아서 해주므로 편하다.
		// 또한 Normalize도 알아서 해주므로 대각선 방향 이동시 빨라지는 것도 고려하지 않아도 된다.

		AddMovementInput(RightDirection, MovementVector.Y);  		// 사용자가 입력한 Y값(위/아래 방향)에 따라 좌/우로 이동
		//  (단, 에디터의 MappingContext의 D,A가 Y축(Swizzle Input Axis Values)으로 설정되어있어야한다.)
	}
}


void AGosooPlayer::Basic_Look(const FInputActionValue& Value)  // 플레이어의 캐릭터의 카메라 회전을 처리하는 함수.
{
	const auto LookAxisVector = Value.Get<FVector2D>();   // 입력된 X, Y 방향(좌우, 위아래)의 값을 2D 벡터로 받아온다.

	if (Controller != nullptr)   // 캐릭터가 조종 가능한 상태일 때만 회전가능
	{
		AddControllerYawInput(LookAxisVector.X);   	// X값은 좌우 입력 -> Yaw(캐릭터가 좌/우로 회전) 변경
		AddControllerPitchInput(LookAxisVector.Y);  // Y값은 위아래 입력 -> Pitch(카메라가 위/아래로 기울어짐) 변경
	}
}

void AGosooPlayer::Basic_Jump(const FInputActionValue& Value)
{
	if (CharacterStateComponent->GetCurrentState() == EPlayerState::Move && IsLanding)  // 현재 플레이어가 지상에 있다면
	{

		GEngine->AddOnScreenDebugMessage(2, 2.0f, FColor::Yellow, TEXT("WOsadas!"));  // 디버그용 

		//GEngine->AddOnScreenDebugMessage(2, 2.0f, FColor::Yellow, TEXT("Jump!"));  // 디버그용 

		CharacterStateComponent->SetCurrentState(EPlayerState::Jump);    // 점프 상태로 바꾸고 점프 횟수 증가시킴

		Jump();     // 점프 실행
		// 점프도 기본적으로 캐릭터 무브먼트 컴포넌트쪽에서 처리가 가능하므로 서버RPC나 ReplicatedUsing등을 사용하지 않고 이렇게 바로 점프를 실행시키면 된다.
	}
	else if (!IsLanding && JumpCount < JumpMaxCount)  // 공중상태이면서 점프횟수가 JumpMaxCount보다 낮다면
	{
		CharacterStateComponent->SetCurrentState(EPlayerState::Jump);   // 점프 상태를 유지하고 점프횟수 증가시킴

		Jump();     // 점프를 한번 더 실행하여 더블 점프 실행
	}
}



void AGosooPlayer::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AGosooPlayer, JumpCount);
	DOREPLIFETIME(AGosooPlayer, IsLanding);
	DOREPLIFETIME(AGosooPlayer, DoubleJumpEnd);
	DOREPLIFETIME(AGosooPlayer, Weapon2);
	DOREPLIFETIME(AGosooPlayer, CanDash);
	DOREPLIFETIME(AGosooPlayer, CanMeleeAttack);
}

//---------------------------------------------------------------------------
// 혹시 뭔가 상호작용을 꼭해서 게임이 진행되어야만 하는게 있다면 서버에서 판단하게끔 하기.
// 그러기 위해서는 그런 상호작용 물체가 있는지 기획서 확인해야함.
//---------------------------------------------------------------------------



//void AGosooPlayer::OnRep_GetItem()
//{
//	if (GetItem)
//	{
//		Stat->WalkSpeed = 1800.0f;
//		GetCharacterMovement()->MaxWalkSpeed = Stat->WalkSpeed;
//	}
//}

void AGosooPlayer::Basic_Crouch(const FInputActionValue& Value)
{
	if (CharacterStateComponent->GetCurrentState() == EPlayerState::Move || CharacterStateComponent->GetCurrentState() == EPlayerState::Crouch)
	{
		Basic_Crouch_Server();
	}
}


void AGosooPlayer::Basic_Crouch_Server_Implementation()     // 앉기 기능 실행하는 함수 정의 
{
	if (CharacterStateComponent->GetCurrentState() == EPlayerState::Move)
	{
		CharacterStateComponent->SetCurrentState(EPlayerState::Crouch);
		
		GetCharacterMovement()->MaxWalkSpeed = Stat->CrouchSpeed;
	}
	else if (CharacterStateComponent->GetCurrentState() == EPlayerState::Crouch)
	{
		CharacterStateComponent->SetCurrentState(EPlayerState::Move);

		GetCharacterMovement()->MaxWalkSpeed = Stat->WalkSpeed;
	}
}


void AGosooPlayer::Basic_Dash(const FInputActionValue& Value)
{
	if ((IsLanding && CharacterStateComponent->GetCurrentState() == EPlayerState::Move) && CanDash)
	{
		Basic_Dash_Server();
	}
}


void AGosooPlayer::Basic_Dash_Server_Implementation()  
{
	if ((IsLanding && CharacterStateComponent->GetCurrentState() == EPlayerState::Move) && CanDash)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, TEXT("Dash Play!!"));

		CharacterStateComponent->SetCurrentState(EPlayerState::Dash);    // 대시 상태로 변경

		CanDash = false;   // CanDash를 false로 바꿔서 대시를 못하게 막음.

		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Magenta, FString::Printf(TEXT("CanDash == %s "), CanDash ? TEXT("True") : TEXT("False")));

		FTimerDelegate DashEnd;                   
		DashEnd.BindWeakLambda(this, [this]()    // 약한 참조 방식의 람다 함수로서 참조하는 객체의 생존 여부를 체크하고, 
												 // 소멸되었다면 람다를 자동으로 무효화하여 콜백이 실행되지 않아 완전 안전한 방식의 함수이다.
			{
				CharacterStateComponent->SetCurrentState(EPlayerState::Move);    // 대시가 끝나면 다시 일반 상태로 변경
			}
		);

		GetWorldTimerManager().SetTimer(DashEndTimer, DashEnd, 1.53f, false);  // 위에서 생성한 람다 함수를 바인딩하여 타이머 실행.

		FTimerDelegate DashCooldown;
		DashCooldown.BindWeakLambda(this, [this]()
			{
				CanDash = true;    // CanDash를 true로 바꿔서 대시를 할 수 있게끔 변경.

				GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Magenta, FString::Printf(TEXT("CanDash == %s"), CanDash ? TEXT("True") : TEXT("False")));
			}
		);

		GetWorldTimerManager().SetTimer(DashCooldownTimer, DashCooldown, 7.0f, false);   // 대시 기능의 쿨타임 시간은 7초
	}
}


void AGosooPlayer::Basic_Interaction_Trace(const struct FInputActionValue& Value)
{
	//GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Magenta, TEXT("RINE"));

	//FHitResult HitInfo;    // 라인트레이스 결과를 담는 변수
	//FVector StartPos = PlayerCameraComponent->GetComponentLocation();    // 라인트레이스 시작 위치
	//FVector EndPos = StartPos + PlayerCameraComponent->GetForwardVector() * 5000.0f;    // 라인트레이스 종료 위치
	//FCollisionQueryParams Params;    // 여러가지 충돌 옵션을 설정할 수 있는 구조체 선언
	//Params.AddIgnoredActor(this);    // 여러가지 설정중에서 이것을 이용하여 자기자신은 라인트레이스 대상에서 무시하게끔 설정.
	//bool bHit = GetWorld()->LineTraceSingleByChannel(HitInfo, StartPos, EndPos, ECC_Visibility, Params);    // 라인트레이스 실행

	//if (bHit && HitInfo.GetActor()->GetClass()->ImplementsInterface(UInteractInterface::StaticClass()))   // 라인트레이스가 맞았고, 맞은 물체가 인터페이스를 구현하고있다면
	//{
	//	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Magenta, TEXT("RINE222"));

	//	Basic_Interaction(HitInfo.GetActor());    // 물체의 상호작용 실행
	//}

	// ----------------------------------------------
	// 기획서상으로는 지상이 아닐때(공중일때)는 입력이 안되다가
	// 지상상태일때 2초뒤부터 입력 받으라는데 이거 맞는지 잘 모르겠다.
	// 정확히는 그렇게하면 약간 게임이 이상하지 않을까 하는 의심.
	// ----------------------------------------------


	if (InteractableActor)
	{
		if (IsLanding && CharacterStateComponent->GetCurrentState() == EPlayerState::Move)
		{
			if (InteractableActor->GetClass()->ImplementsInterface(UInteractInterface::StaticClass()))
			{
				IInteractInterface::Execute_InteractInterfaceFunction(InteractableActor, this);
			}
		}
	}
}


void AGosooPlayer::Basic_Interaction_Implementation(AActor* InteractablActor)    // 상호작용하는 함수 정의
{
	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Magenta, TEXT("RINE333"));

	IInteractInterface::Execute_InteractInterfaceFunction(InteractablActor, this);     // 상호작용 가능한 오브젝트의 인터페이스 실행

}

void AGosooPlayer::Basic_MeleeAttack(const FInputActionValue& Value)
{
	if (CharacterStateComponent->GetCurrentState() != EPlayerState::Move || !CanMeleeAttack)
	{
		return;
	}

	if (PlayerAnimInstance && MeleeAttackMontage)
	{
		Basic_MeleeAttack_Server();
	}
}

void AGosooPlayer::Basic_MeleeAttack_Server_Implementation()    // 근접공격키를 누르면 실행할 서버RPC 정의
{
	if (CharacterStateComponent->GetCurrentState() != EPlayerState::Move || !CanMeleeAttack)
	{
		return;
	}

	if (PlayerAnimInstance && MeleeAttackMontage)     // 애님인스턴스와 근접공격 몽타주가 있다면
	{
		CanMeleeAttack = false;

		OnRep_CanMeleeAttack();     // 근접 공격 애니메이션 실행 및 속도 낮춤.

		MeleeAttackHitCheck();     // 근접공격 판정 실행

		FTimerDelegate MeleeAttackCooldown;
		MeleeAttackCooldown.BindWeakLambda(this, [this]()
			{
				CanMeleeAttack = true;

				OnRep_CanMeleeAttack();   // 속도 정상화.
			}
		);

		GetWorldTimerManager().SetTimer(MeleeAttackCooldownTimer, MeleeAttackCooldown, 4.0f, false);
		
		// ---------------------- 꼭 해야할 것 ---------------------------------------
		//  기획서를 보면 근접공격 동안은 90% 속도로만 이동하게끔 되어 있다.
		//  지금 당장은 몽타주로 애니메이션 플레이하게끔 하는 코드라서 이동속도 자체는 낮추긴한다.
		//  그런데 앉기 상태 바꿔서 이동속도를 변경하는 코드같은것과는 달리 이건 상태 변경으로 바꾸는게 아니라서
		//	애니메이션이 끝나고 근접공격 기능의 쿨타임이 돌때까지 계속 느리게 이동한다.
		//  따라서 추가적으로 몽타주가 아니라 애니메이션 블루프린트 방식으로 할건지 등을 생각해서 속도 변경 코드를 고쳐야 한다.
		// --------------------------------------------------------------------------
	}
	else   // 근접공격 몽타주가 없다면
	{
		GEngine->AddOnScreenDebugMessage(6, 2.0f, FColor::Blue, TEXT("No Montage"));
	}


	// ------------------------------------------------- 원본 ---------------------------------------
	// 돌려놓을때 서버RPC 말고 일반적인 멤버 함수처럼 돌려놔야함.
	//FHitResult HitInfo;    // 라인트레이스 결과를 담는 변수
	//FVector StartPos = PlayerCameraComponent->GetComponentLocation();    // 라인트레이스 시작 위치
	//FVector EndPos = StartPos + PlayerCameraComponent->GetForwardVector() * 5000.0f;    // 라인트레이스 종료 위치
	//FCollisionQueryParams Params;    // 여러가지 충돌 옵션을 설정할 수 있는 구조체 선언
	//Params.AddIgnoredActor(this);    // 여러가지 설정중에서 이것을 이용하여 자기자신은 라인트레이스 대상에서 무시하게끔 설정.
	//bool bHit = GetWorld()->LineTraceSingleByChannel(HitInfo, StartPos, EndPos, ECC_Visibility, Params);    // 라인트레이스 실행


	//DrawDebugLine(GetWorld(), StartPos, EndPos, FColor::Green, false, 2.0f);

	//FDamageEvent DamageEvent;

	//if (bHit && Cast<ADrone_Enemy>(HitInfo.GetActor()))
	//{	
	//	GEngine->AddOnScreenDebugMessage(2, 2.0f, FColor::Yellow, TEXT("Drone!"));  // 디버그용 
	//	HitInfo.GetActor()->TakeDamage(100.0f, DamageEvent, GetController(), this);
	//}
	//----------------------------------------------- 원본 끝 ---------------------------------------------
}

void AGosooPlayer::Basic_Attack()    // 근접공격을 실행할 NetMulticastRPC 정의
{
	GEngine->AddOnScreenDebugMessage(7, 2.0f, FColor::Blue, TEXT("Attack"));

	if (Weapon2) 
	{
		GEngine->AddOnScreenDebugMessage(6, 2.0f, FColor::Red, TEXT("Fire"));
		Weapon2->Attack();
	}
}


void AGosooPlayer::OnRep_CanMeleeAttack()
{
	if (!PlayerAnimInstance && MeleeAttackMontage)   // BeginPlay에서 애님인스턴스 변수를 담는다. 
													 // 따라서 클라이언트쪽에서 캐릭터가 생성되기전에 서버쪽에서 먼저 CanMeleeAttack을 
													 // 변경해버리면 OnRep함수가 실행되면서 아직 nullptr 상태인 PlayerAnimInstance를 
													 // 참조해버리면서 FatalError가 뜬다. 따라서 여기서도 PlayerAnimInstance가 있는지
													 // 체크를 해야한다.
													 // 보통은 OnRep함수들 쪽에서 nullptr인 변수들을 참조하면서 많은 문제가 생기니
													 // OnRep함수들쪽에 방어코드를 많이 적어두도록 하자.
	{
		return;
	}

	if (!CanMeleeAttack)
	{
		PlayerAnimInstance->Montage_Play(MeleeAttackMontage, 1.0f);

		GetCharacterMovement()->MaxWalkSpeed = Stat->WalkSpeed * 0.3f;
	}
	else if (CanMeleeAttack)
	{
		GetCharacterMovement()->MaxWalkSpeed = Stat->WalkSpeed;
	}
}

void AGosooPlayer::MeleeAttackHitCheck()
{
	MeleeAttackHitBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);    // 근접공격 판정 시작

	FTimerDelegate MeleeAttackHitCheckFunction;              
	MeleeAttackHitCheckFunction.BindWeakLambda(this, [this]
		{
			for (ABase_Enemy* EnemyObj : HitObjs) 
			{
				FDamageEvent DamageEvent;
				EnemyObj->TakeDamage(100.0f, DamageEvent, GetController(), this);    // 근접공격에 맞은 적 HP 감소
			}

			HitObjs.Empty();     // 배열을 비운다.
			
			MeleeAttackHitBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);   // 다시 충돌 판단하지 않게끔 변경.
		}
	);

	GetWorldTimerManager().SetTimer(MeleeAttackHitCheckTimer, MeleeAttackHitCheckFunction, 0.3f, false);  // 근접 공격 실행후 0.3초 뒤 판단

	// ----------------------- 해야 할 것------------------------------------
	// 현재는 내가 믹사모에서 가져온 애니메이션을 기준으로해서 0.3초를 기준으로 잡았다.
	// 그러나 기획서에는 1.5초 뒤에 근접공격 피격을 판단으로 하자고 한다.
	// 그래서 애니메이션을 조정해야할지 아니면 기획서를 조정해야 할지 다시 한번 생각해야한다.
	// 그리고 0.3초로 한다고 하면 시간이 너무 짧아서 EndOverlap 되기전에 여기에 있는
	// 타이머 함수로 인해 배열이 비워진다. 그래서 EndOverlap이 사실상 의미가 없다.
	// 만약 기획이 수정된다고 한다면 EndOverlap을 지워야할 가능성이 매우 높다.
	//----------------------------------------------------------------------
}

void AGosooPlayer::MeleeHitBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{	
	if (const auto HitObj = Cast<ABase_Enemy>(OtherActor))
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Magenta, FString::Printf(TEXT("HitObjNum = %d"), HitObjs.Num()));

		if (!HitObjs.Contains(HitObj))   // 배열안에 하나도 포함되어 있지 않다면 (중복해서 넣지 않게끔 하기 위해서)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Magenta, FString::Printf(TEXT("HitObjNum = %d"), HitObjs.Num()));

			HitObjs.Add(HitObj);   // 배열에 넣는다.

			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Magenta, FString::Printf(TEXT("HitObjNum = %d"), HitObjs.Num()));
		}
	}
}

void AGosooPlayer::MeleeHitBoxEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (const auto HitObj = Cast<ABase_Enemy>(OtherActor))
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Cyan, FString::Printf(TEXT("Exit_HitObjNum = %d"), HitObjs.Num()));

		if (HitObjs.Contains(HitObj))     // 배열에 있다면
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Cyan, FString::Printf(TEXT("Exit_HitObjNum = %d"), HitObjs.Num()));

			HitObjs.Remove(HitObj);    // 배열에서 제거한다.

			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Cyan, FString::Printf(TEXT("Exit_HitObjNum = %d"), HitObjs.Num()));
		}
	}
}

//  -------------------------------------------------
//  기획서를 보면 감정표현은 IDle상태에서만 가능하다고 한다.
//  그거 자체는 구현이 어려운게 아닌데, 그 외에 상세한 것.
//  예를들면 "~"를 눌러서 감정표현을 고르는 카테고리 창이 뜨고나서
//  Idle상태가 아니라면 춤버튼을 눌러도 실행되지 않는데, 그러면 
//  카테고리 창은 지워지게 할 것인지, 아니면 그대로 놔둘것인지
//  아니면 플레이어에게 Idle 상태에서만 가능하다는 문구를 띄운다던지 할 것인지가  궁금
//  -------------------------------------------------


void AGosooPlayer::EmotionControl()    // "~"키 누를때마다 감정표현 설명 UI 껐다 켤수있는 함수 정의.
{
	if (PersonalPlayerUI)
	{
		PersonalPlayerUI->WatchEmotionScreen();     
	}
}


void AGosooPlayer::Emotion1()
{
	if (PersonalPlayerUI->GetIsEmotionOverlayOpen() == true)      //  감정표현 설명 UI가 켜져있을때만 감정표현 실행 가능. 
	{
		PersonalPlayerUI->WatchEmotionScreen();        // 감정표현 설명 UI 끄기.
		
		Emotion1_Server();       // 감정표현1 실행.
	}
}

void AGosooPlayer::Emotion1_Server_Implementation()
{
	Emotion1_NetMulticast();      // 감정표현은 게임성에 크게 영향을 끼치는게 아니므로 NetMulticast로 몽타주를 실행.
}

void AGosooPlayer::Emotion1_NetMulticast_Implementation()
{
	if(PlayerAnimInstance)
	{
		PlayerAnimInstance->Montage_Play(Emotion1Montage, 1.0f);   // 감정표현1 시작
	}
}

void AGosooPlayer::Emotion2()
{
	if (PersonalPlayerUI->GetIsEmotionOverlayOpen() == true)
	{
		PersonalPlayerUI->WatchEmotionScreen();

		Emotion2_Server();
	}
}

void AGosooPlayer::Emotion2_Server_Implementation()
{
	Emotion2_NetMulticast();
}

void AGosooPlayer::Emotion2_NetMulticast_Implementation()
{
	if (PlayerAnimInstance)
	{
		PlayerAnimInstance->Montage_Play(Emotion2Montage, 1.0f);   // 감정표현1 시작
	}
}

void AGosooPlayer::Emotion3()
{
	if (PersonalPlayerUI->GetIsEmotionOverlayOpen() == true)
	{
		PersonalPlayerUI->WatchEmotionScreen();

		Emotion3_Server();
	}
}

void AGosooPlayer::Emotion3_Server_Implementation()
{
	Emotion3_NetMulticast();
}

void AGosooPlayer::Emotion3_NetMulticast_Implementation()
{
	if (PlayerAnimInstance)
	{
		PlayerAnimInstance->Montage_Play(Emotion3Montage, 1.0f);   // 감정표현1 시작
	}
}

//-------------------------------------------
// 기획서상으로는 기본 상태일때만 지도를 켜게 해놨다.
// 그러나 문제는 지도를 켜도 캐릭터가 움직일수 있는데, 
// 여기 조건에 의하면 기본상태일때만 켜고 끄는게 가능하므로,
// 지도를 켠 상태로 앉기라던가 다른 상태일때는 이 함수가 작동하지 않는다.
// 따라서 지도를 켠채로 다른 상태일때, 피격당해 체력이 깎여도 지도가 꺼지지 않는다.
// 그래서 지도를 켜면 못움직이게 해야하는건지, 혹은 움직일수 있다고 한다면 내가 어떻게든 한번 고쳐봐야한다.
//-------------------------------------------
void AGosooPlayer::MapFunction()
{
	if (CharacterStateComponent->GetCurrentState() != EPlayerState::Move)
	{
		return;
	}

	if (!IsMapUIOpen)    // 맵이 안켜져있으면
	{
		if (!PersonalMapUI)   // 아직 만들어둔 맵UI가 없다면 
		{
			if (PersonalMapUIClass)    // 만들어야 할 맵UI를 할당했는지 확인하고,
			{
				PersonalMapUI = CreateWidget<UMapUI>(GetWorld(), PersonalMapUIClass);    // 맵UI 생성

				PersonalMapUI->AddToViewport();

				IsMapUIOpen = !IsMapUIOpen;    // 변수를 반대로 변경.
			}
		}
		else      // 이미 만들어둔 맵UI가 있다면
		{
			PersonalMapUI->AddToViewport();

			IsMapUIOpen = !IsMapUIOpen;    // 변수를 반대로 변경.
		}
	}
	else    // 맵이 켜져 있다면
	{
		PersonalMapUI->RemoveFromParent();   // 맵UI를 제거.

		IsMapUIOpen = !IsMapUIOpen;   // 변수를 반대로 변경.
	}
}

void AGosooPlayer::OptionSelectOpen()
{	
	if (PersonalOptionSelectUI)
	{
		PersonalOptionSelectUI->AddToViewport();
	}
	else
	{
		if (PersonalOptionSelectUIClass)
		{
			PersonalOptionSelectUI = CreateWidget<UOptionSelectUI>(GetWorld(), PersonalOptionSelectUIClass);

			PersonalOptionSelectUI->SetOwnerPlayer(this);

			PersonalOptionSelectUI->AddToViewport();
		}
	}
}


void AGosooPlayer::SetDoubleJumpEnd()   // 애니메이션시퀀스의 노티파이로 실행할 것임.
{
	if (HasAuthority())
	{
		DoubleJumpEnd = true;    // 더블 점프까지 실행 완료했다고 알려줌.
	}
}


void AGosooPlayer::SetWeapon2()
{
	TArray<AActor*> ChildActors;
	GetAllChildActors(ChildActors);

	if (ChildActors.Num() != 0)
	{
		GEngine->AddOnScreenDebugMessage(1, 2.0f, FColor::Red, TEXT("ChildActors"));
		
		for (AActor* ChildActor : ChildActors)
		{
			if (AABaseWeapon* a = Cast<AABaseWeapon>(ChildActor))
			{
				GEngine->AddOnScreenDebugMessage(2, 2.0f, FColor::Red, TEXT("ABaseWeapon Is True"));

				Weapon2 = a;

				Weapon2->SetOwner(this);

				return;
			}
		}
	}
}



void AGosooPlayer::TestAction_Implementation()    // hp 테스트를 위한 것이니 나중에 지울것
{
	Stat->HP -= 20.0f;    // HP 20 깎음.

	GEngine->AddOnScreenDebugMessage(16, 5.0f, FColor::Emerald, FString::Printf(TEXT("%f"), Stat->HP));

	Stat->OnRep_HP();
}

 
float AGosooPlayer::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	return 0.0f;
}
