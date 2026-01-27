// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerSkill.h"
#include "KimSkill.generated.h"

/**
 * 
 */

UENUM(BlueprintType, meta = (ScriptName = "KimSkillState"))
enum class EKimSkillState : uint8  // uint8로 지정했으므로 256개까지 열거 가능.
{
	Expanding,		     // 기본 무기 사용중인 상태
	Holding,     // 스킬무기 사용중인 상태
	Shrinking          // 공격중인 상태
};

UCLASS()
class GOSOO_API UKimSkill : public UPlayerSkill
{
	GENERATED_BODY()
	
	UKimSkill();

	void Skill_1_Excute() override;

	void Skill_2_Excute() override;

	//UPROPERTY(EditAnywhere, Category = "ReferencesRequired|Skill")
	//TObjectPtr<class AActor> SandevistanMesh;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class USphereComponent> SandevistanDomain;

	float CurrentDomainSize = 0.0f;

	UPROPERTY(EditAnywhere, Category = "ReferencesRequired|Skill")
	float TargetDomainSize = 300.0f;

	UPROPERTY(EditAnywhere, Category = "ReferencesRequired|Skill")
	float GrowDuration = 1.0f;

	UPROPERTY(EditAnywhere, Category = "ReferencesRequired|Skill")
	float GrowInterval = 0.02f;

	float CurrentHoldingTime = 0.0f;

	int32 Steps;    // 총 몇번의 틱으로 나눠서 실행할 것인지 설정하는 변수.

	float RadiusStep;   // 틱에 맞게끔 얼마나 사이즈를 키우고 줄여야하는지 크기를 설정하는 변수

	FTimerHandle SandevistanTimer;

	EKimSkillState CurrentKimSkillState = EKimSkillState::Expanding;

	UPROPERTY(EditAnywhere)
	TObjectPtr<class UStaticMeshComponent> SandevistanEffect;

protected:
	virtual void BeginPlay() override;
};
