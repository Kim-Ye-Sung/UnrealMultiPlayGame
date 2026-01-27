// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerSkill.h"
#include "RavenSkill.generated.h"

/**
 * 
 */
UCLASS()
class GOSOO_API URavenSkill : public UPlayerSkill
{
	GENERATED_BODY()
	
	URavenSkill();

	void Skill_1_Excute() override;
	
	void Skill_2_Excute() override;
	
	UPROPERTY(EditAnywhere, Category = "ReferencesRequired|SkillMontage")
	TObjectPtr<UAnimMontage>SkillMontage;

	UPROPERTY(EditAnywhere, Category = "ReferencesRequired|SkillMontage")
	TObjectPtr<UAnimMontage>SkillMontage2;

	UPROPERTY()
	int32 Combo = 0;
	

public:
	UPROPERTY()
	bool HasGoalTransform = false;

	UPROPERTY()
	FVector SkillGoalLocation;




	// -----------------------------------------------
	//TArray<FNavPathPoint>PathPoints;  // 경로상의 모든 포인트들을 저장하는 배열.  
	//										// 이건 UPROPERTY()와 함께 사용할 수 없는 타입이다. 선언하면 보라색 줄 그인다.

	//UPROPERTY()
	//int32 CurrentPathIndex;   // 현재 향하고 있는 경로 포인트의 인덱스

	//UPROPERTY()
	//FTimerHandle MoveTimerHandle;   // 이동을 처리하는 타이머의 핸들(타이머 중지용)

	//UPROPERTY()
	//float TotalPathLength;    // 전체 경로의 길이(미터 단위)


	//UFUNCTION()
	//void MoveToLocationWithNavMesh();

	//UFUNCTION()
	//void FollowPathTick();


	// -----------------------------------------------------------
	UPROPERTY()
	TArray<FVector> PathPoints;

	UPROPERTY()
	int32 CurrentPathIndex;

	UPROPERTY()
	FTimerHandle MoveTimerHandle;

	UPROPERTY()
	float MoveSpeed;

	UFUNCTION()
	void StartMoveToGoalLocation();

	UFUNCTION()
	void FollowPath();
};
