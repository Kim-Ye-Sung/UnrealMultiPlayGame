// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterState.h"
#include "Net/UnrealNetwork.h"
#include "GosooPlayer.h"
#include "PlayerStat.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values for this component's properties
UCharacterState::UCharacterState()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	
	PrimaryComponentTick.bCanEverTick = true;



	SetIsReplicatedByDefault(true);
	// ...
}


// Called when the game starts
void UCharacterState::BeginPlay()
{
	Super::BeginPlay();

	OwnerCharacter = Cast<AGosooPlayer>(GetOwner());

	// ...
}


// Called every frame
void UCharacterState::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}


void UCharacterState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UCharacterState, CurrentState);
}

void UCharacterState::SetCurrentState_Implementation(EPlayerState NewState)    // 플레이어의 상태를 변경하는 함수 정의.
{
	CurrentState = NewState;  

	if (NewState == EPlayerState::Jump)    // 새로운 상태가 점프 상태라면
	{
		OwnerCharacter->JumpCount++;       // 플레이어의 점프 횟수를 올림
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Emerald, TEXT("StateComponent JumpCountPlus!!"));
	}
}

void UCharacterState::OnRep_CurrentState()
{
	if (OwnerCharacter == nullptr)
	{
		return;
	}

	switch (CurrentState)   // 상태에 따라 캐릭터의 속도등을 변경
	{
	case EPlayerState::Move:
		OwnerCharacter->GetCharacterMovement()->MaxWalkSpeed = OwnerCharacter->GetPlayerStat()->WalkSpeed;
		break;
	case EPlayerState::Crouch:
		OwnerCharacter->GetCharacterMovement()->MaxWalkSpeed = OwnerCharacter->GetPlayerStat()->CrouchSpeed;
		break;
	case EPlayerState::Jump:
		break;
	case EPlayerState::Dash:
		break;
	default:
		break;
	}
}