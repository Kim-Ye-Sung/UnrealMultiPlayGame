// Fill out your copyright notice in the Description page of Project Settings.


#include "EvelynnSkill.h"
#include "Net/UnrealNetwork.h"
#include "GosooPlayer.h"

void UEvelynnSkill::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

void UEvelynnSkill::Skill_1_Excute()  // 스킬 키를 누르면 실행할 함수 정의
{
	if (AGosooPlayer* OwnerPlayer = Cast<AGosooPlayer>(GetOwner()))
	{
		if (OwnerPlayer && OwnerPlayer->PlayerAnimInstance)
		{
			OwnerPlayer->PlayerAnimInstance->Montage_Play(Unarmed_Equip_Over, 1.0f);  // 칼 뽑는 애니메이션 실행
		}
	}

	FTimerDelegate Skill_1_ExcuteDelegate;
	Skill_1_ExcuteDelegate.BindWeakLambda(this, [this]()   // 스킬 지속 시간이 끝나면 실행할 내용들
		{
			SlashChange = true; // 다음 스킬 시전시 횡베기부터 시작할 수 있게끔 초기화.

			if (AGosooPlayer* OwnerPlayer = Cast<AGosooPlayer>(GetOwner()))
			{
				if (OwnerPlayer && OwnerPlayer->PlayerAnimInstance)
				{
					OwnerPlayer->PlayerAnimInstance->Montage_Play(Unarmed_Equip_Over, 1.0f);    
					OwnerPlayer->PlayerAnimInstance->Montage_JumpToSection(TEXT("ReverseUnarmed"), Unarmed_Equip_Over);   // 칼 다시 넣기
				}
			}
		}
	);

	GetWorld()->GetTimerManager().SetTimer(Skill_1_ExcuteTimer, Skill_1_ExcuteDelegate, Skill_One_ActiveTime, false);  // 스킬 지속 시간 타이머 실행
}

void UEvelynnSkill::Skill_2_Excute()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Cyan, TEXT("EvelynnSkill 222 !!!"));
}

void UEvelynnSkill::SlashAction()
{
	CurrentEvelynnSkillState = EEvelynnSkillState::Attack;   // 칼을 휘두르고 있는 상태로 변경

	AGosooPlayer* OwnerPlayer = Cast<AGosooPlayer>(GetOwner());

	if (OwnerPlayer && OwnerPlayer->PlayerAnimInstance)    // 횡베기와 종베기를 번갈아가면서 실행
	{
		if (SlashChange)
		{
			OwnerPlayer->PlayerAnimInstance->Montage_Play(SlashMontage, 1.0f);
			OwnerPlayer->PlayerAnimInstance->Montage_JumpToSection(TEXT("VerticalSlash"), SlashMontage);
			SlashChange = false;
		}
		else
		{
			OwnerPlayer->PlayerAnimInstance->Montage_Play(SlashMontage, 1.0f);
			OwnerPlayer->PlayerAnimInstance->Montage_JumpToSection(TEXT("HorizontalSlash"), SlashMontage);
			SlashChange = true;
		}
	}
}
