// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerSkill.h"
#include "EvelynnSkill.generated.h"

/**
 * 
 */
UENUM(BlueprintType, meta = (ScriptName = "EvelynnSkillState"))
enum class EEvelynnSkillState : uint8  // uint8로 지정했으므로 256개까지 열거 가능.
{
	Normal,		     // 기본 무기 사용중인 상태
	WeaponUsing,     // 스킬무기 사용중인 상태
	Attack          // 공격중인 상태
};


UCLASS()
class GOSOO_API UEvelynnSkill : public UPlayerSkill
{
	GENERATED_BODY()

	virtual void GetLifetimeReplicatedProps(TArray <FLifetimeProperty>& OutLifetimeProps) const override;

	void Skill_1_Excute() override;

	void Skill_2_Excute() override;

	FTimerHandle Skill_1_ExcuteTimer;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	bool SlashChange = true;    // 항상 횡베기부터 시작한다.

	UPROPERTY(EditAnywhere, Category = "ReferencesRequired|SkillMontage")
	TObjectPtr<class UAnimMontage> Unarmed_Equip_Over;

	UPROPERTY(EditAnywhere, Category = "ReferencesRequired|SkillMontage")
	TObjectPtr<UAnimMontage> SlashMontage;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	EEvelynnSkillState CurrentEvelynnSkillState = EEvelynnSkillState::Normal;

	UPROPERTY(EditAnywhere, Category = "ReferencesRequired|SkillEffect")
	TObjectPtr<class UStaticMesh> BarrierMesh;

	UPROPERTY(EditAnywhere, Category = "ReferencesRequired|SkillEffect")
	TObjectPtr<UStaticMesh> SlashEffect;

public:
	FORCEINLINE void SetSlashChange() { SlashChange = !SlashChange; }    // 횡베기 종베기를 바꿔줌으로서 번갈아가면서 실행하게끔 한다.

	FORCEINLINE EEvelynnSkillState GetCurrentEvelynnSkillState() const { return CurrentEvelynnSkillState; }

	UFUNCTION()
	void SlashAction();
};
