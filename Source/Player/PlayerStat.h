// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlayerStat.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FHP_Change, float, HP_Stat, float, Max_HP_Stat);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class GOSOO_API UPlayerStat : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UPlayerStat();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Basic_Stat")
	float MaxHP = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Basic_Stat", ReplicatedUsing = "OnRep_HP")
	float HP = MaxHP;         // 캐릭터 체력

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Basic_Stat")
	float WalkSpeed = 350.0f;    // 캐릭터 기본 이동속도 

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Basic_Stat")
	float CrouchSpeed = WalkSpeed * 0.4f;     // 캐릭터 앉기 속도

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Basic_Stat")
	float RunSpeed = 450.0f;     // 캐릭터 달리기 속도


	virtual void GetLifetimeReplicatedProps(TArray <FLifetimeProperty>& OutLifetimeProps) const override;


	UFUNCTION()
	void OnRep_HP();

	UPROPERTY()
	TObjectPtr<class AGosooPlayer> OwnerCharacter;

	FHP_Change HP_Change_Function;
};
