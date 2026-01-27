// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerSkill.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GosooPlayer.h"
#include "Net/UnrealNetwork.h"
//#include "Engine/World.h"
//#include "TimerManager.h"

// Sets default values for this component's properties
UPlayerSkill::UPlayerSkill()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	SetIsReplicatedByDefault(true);   // 액터 컴포넌트 자체가 네트워크에서 복제될지 여부를 결정.
									  // 이것을 true로 설정하지 않으면 복제변수값을 변경되고 복제되어 오지 않아서 문제가 발생한다.
									 

	// ...
}


// Called when the game starts
void UPlayerSkill::BeginPlay()
{
	Super::BeginPlay();

	// ...

	//const ACharacter* PlayerCharacter = Cast<ACharacter>(GetOwner());

	OwnerCharacter = Cast<AGosooPlayer>(GetOwner());

	Skill_One_RemainingTime = Skill_One_Cooldown;
	Skill_Two_RemainingTime = Skill_Two_Cooldown;
	
}


// Called every frame
void UPlayerSkill::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
	
	// 스킬 쿨타임 계산을 복제 변수로 두어 확실하게 동기화를 할 수 있었으나, 
	// 그렇게 한다면 Tick마다 복제가 일어나게 되어 네트워크상에 부담이 늘어나게 된다.
	// 따라서 타이머로 맞춰서 Can_Skill이라는 변수로 스킬의 사용가능 여부만 서버에서 판단하고 복제하게 하고, 
	// UI에 표시될 스킬 쿨타임 계산은 아래와 같이 로컬플레이어가 각자 계산하게 한다.


	if (!OwnerCharacter || !OwnerCharacter->IsLocallyControlled())   // 스킬컴포넌트의 오너가 로컬플레이어가 아니라면 Tick을 실행하지 않는다.
	{
		return;
	}


	if (!Can_Skill_One)    // 스킬1을 사용할 수 없을 때, 남은 쿨타임을 계산한다.
	{
		Skill_One_RemainingTime += GetWorld()->GetDeltaSeconds();

		Skill_One_Change.Broadcast(Skill_One_RemainingTime, Skill_One_Cooldown);
	}

	if (!Can_Skill_Two)    // 스킬2를 사용할 수 없을 때, 남은 쿨타임을 계산한다.
	{
		Skill_Two_RemainingTime += GetWorld()->GetDeltaSeconds();

		Skill_Two_Change.Broadcast(Skill_Two_RemainingTime, Skill_Two_Cooldown);
	}
}

void UPlayerSkill::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UPlayerSkill, Can_Skill_One);
	DOREPLIFETIME(UPlayerSkill, Can_Skill_Two);
	DOREPLIFETIME(UPlayerSkill, CanSkill1);
}



void UPlayerSkill::Skill_1()
{
	if (Skill_One_Active && CanSkill1)    // 스킬1이 액티브 스킬이면서 발동 가능한 상태면
	{ 
		Skill_1_Server();     // 스킬 발동.
	}
}

void UPlayerSkill::Skill_2()
{
	if (Skill_Two_Active && Can_Skill_Two)
	{
		Skill_2_Server();
	}
}

void UPlayerSkill::Skill_1_Server_Implementation()
{
	if (Skill_One_Active && CanSkill1)   // 스킬1이 액티브 스킬이면서 쿨타임이 다 채워졌다면 스킬 실행.
	{
		Skill_1_Excute();  // 스킬 실행

		// 스킬키를 한번 더 눌러서 재사용해야하는 콤보스킬이 아니라면 쿨타임 실행
		// (따라서 이 조건문에 의해 콤보스킬이면 스킬키를 더 누를 수 있음)
		if (!ComboSkill)    
		{
			CanSkill1 = false;

			Can_Skill_One = false;    // 스킬 재사용 불가능하게 바꾼다.

			OnRep_CanSkill_One();     // 쿨타임 시작

			FTimerDelegate Skill_One_CooldownFunction;    // 쿨타임이 다 채워졌다면     
			Skill_One_CooldownFunction.BindWeakLambda(this, [this]
				{
					CanSkill1 = true;

					Can_Skill_One = true;     // 스킬 사용 가능하게 변경

					OnRep_CanSkill_One();    // 확실하게 쿨타임 프로그래스바를 100%로 변경
				} 
			); 

			GetWorld()->GetTimerManager().SetTimer(Skill_One_CooldownTimer, Skill_One_CooldownFunction, Skill_One_Cooldown, false);
		}
		else if (ComboSkill && CoolDown_On)  // 콤보 스킬인데, 쿨타임을 계산하라는 명령이 떨어진다면
		{
			CanSkill1 = false;    // 스킬 키 사용 못하게 변경

			CoolDown_On = false;   // 딱 한번만 쿨타임을 실행하면 되므로 다시는 이 조건문에 들어오지 못하도록 막아둔다.

			Can_Skill_One = false;   // 스킬 사용을 불가능하게 바꾼다.
			
			OnRep_CanSkill_One();     // 쿨타임 시작

			FTimerDelegate Skill_One_CooldownFunction;
			Skill_One_CooldownFunction.BindWeakLambda(this, [this]
				{
					CanSkill1 = true;

					ComboEnd = false;

					Can_Skill_One = true;

					OnRep_CanSkill_One();
				}
			);

			GetWorld()->GetTimerManager().SetTimer(Skill_One_CooldownTimer, Skill_One_CooldownFunction, Skill_One_Cooldown, false);

			if (!ComboEnd)    // 쿨타임은 돌고 있지만 아직 스킬 키를 더 누를 수 있는 스킬이라면
			{
				CanSkill1 = true;   // 스킬 키를 누를 수 있게 변경

				// 만약 쿨타임 돌 때 스킬 키도 못누르게 하려면 각 캐릭터의 Skill_1_Excute 함수에서 ComboEnd를 반드시 true로 해줘야한다.
			}
		}
	}
}

void UPlayerSkill::Skill_2_Server_Implementation()
{
	if (Skill_Two_Active && Can_Skill_Two)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Magenta, TEXT("Skill_Two!"));  // 디버그용 

		Can_Skill_Two = false;

		OnRep_CanSkill_Two();

		Skill_2_Excute();

		FTimerDelegate Skill_Two_CooldownFunction;
		Skill_Two_CooldownFunction.BindWeakLambda(this, [this]
			{
				Can_Skill_Two = true;

				OnRep_CanSkill_Two();

				GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, TEXT("You Cans Skill_Two!"));  // 디버그용 
			}
		);

		GetWorld()->GetTimerManager().SetTimer(Skill_Two_CooldownTimer, Skill_Two_CooldownFunction, Skill_Two_Cooldown, false);
	}
}

void UPlayerSkill::OnRep_CanSkill_One()
{
	if (Can_Skill_One)   // 스킬1을 사용할 수 있게 된다면 
	{
		Skill_One_RemainingTime = Skill_One_Cooldown;    // 스킬쿨타임을 꽉 채워서 사용할 수 있게끔 표시   

		Skill_One_Change.Broadcast(Skill_One_RemainingTime, Skill_One_Cooldown);
	}
	else   // 스킬1을 사용할 수 없다면
	{
		Skill_One_RemainingTime = 0.0f;     // 스킬 쿨타임을 0초로 해서 계산 시작

		Skill_One_Change.Broadcast(Skill_One_RemainingTime, Skill_One_Cooldown);
	}
}

void UPlayerSkill::OnRep_CanSkill_Two()
{
	if (Can_Skill_Two)   // 스킬2를 사용할 수 있게 된다면 
	{
		Skill_Two_RemainingTime = Skill_Two_Cooldown;   // 스킬쿨타임을 꽉 채워서 사용할 수 있게끔 표시

		Skill_Two_Change.Broadcast(Skill_Two_RemainingTime, Skill_Two_Cooldown);
	}
	else    // 스킬2를 사용할 수 없다면
	{
		Skill_Two_RemainingTime = 0.0f;    // 스킬 쿨타임을 0초로 해서 계산 시작

		Skill_Two_Change.Broadcast(Skill_Two_RemainingTime, Skill_Two_Cooldown);
	}
}
