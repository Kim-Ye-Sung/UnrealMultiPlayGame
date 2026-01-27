// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GosooPlayer.h"
#include "RavenPlayer.generated.h"

/**
 * 
 */
UCLASS()
class GOSOO_API ARavenPlayer : public AGosooPlayer
{
	GENERATED_BODY()
	
	ARavenPlayer();

	UPROPERTY(EditAnywhere, Category = "ReferencesRequired|SkillObject")
	TSubclassOf<class ARavenSkillObject_Marker>SkillObject;     // 레이븐의 스킬 사용시 투적하여 마킹할 물체를 블루프린트로 만들어서 여기에 할당할 예정

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<ARavenSkillObject_Marker>SpawnedSkillObject;

protected:
	virtual void BeginPlay() override;

public:
	UFUNCTION(BlueprintCallable)
	void DetachSkillObject();

	UFUNCTION()
	void SpawnSkillObject();
};
