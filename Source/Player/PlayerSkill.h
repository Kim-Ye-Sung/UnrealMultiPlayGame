// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "PlayerSkill.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FSkillOneDele, float, RemainingTime1, float, Cooldown1);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FSkillTwoDele, float, RemainingTime2, float, Cooldown2);

UCLASS(Abstract, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class GOSOO_API UPlayerSkill : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPlayerSkill();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;



private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ReferencesRequired|SkillButton", meta =(AllowPrivateAccess = "true"))
	TObjectPtr<class UInputMappingContext> SkillMappingContext;

	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ReferencesRequired|SkillButton", meta = (AllowPrivateAccess = "true"))
	//TObjectPtr<class UInputAction> Skill_One;

	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ReferencesRequired|SkillButton", meta = (AllowPrivateAccess = "true"))
	//TObjectPtr<class UInputAction> Skill_Two;

protected:

	UPROPERTY()
	TObjectPtr<class AGosooPlayer> OwnerCharacter;

	virtual void GetLifetimeReplicatedProps(TArray <FLifetimeProperty>& OutLifetimeProps) const override;


	UFUNCTION(Server, Reliable)
	void Skill_1_Server();

	UFUNCTION(Server, Reliable)
	void Skill_2_Server();

	
	virtual void Skill_1_Excute() PURE_VIRTUAL(UPlayerSkill::Skill_1_Excute, ;;);  // 이러면 무조건 자식클래스에서 구현해야하는 순수 가상함수로 선언하는것
																				   // 실제로 PlayerSkill.cpp 파일을 보면 함수 정의 자체가 없음.

	virtual void Skill_2_Excute() PURE_VIRTUAL(UPlayerSkill::Skill_2_Excute, ;;);

	UPROPERTY(EditAnywhere, Category = "SkillInfo")
	bool Skill_One_Active = true;      // 스킬1이 액티브 스킬이냐를 결정하는 변수

	UPROPERTY(EditAnywhere, Category = "SkillInfo")
	bool Skill_Two_Active = true;      // 스킬2가 액티브 스킬이냐를 결정하는 변수

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SkillInfo", ReplicatedUsing = "OnRep_CanSkill_One")
	bool Can_Skill_One = true;      // 스킬1이 사용 가능한 상태인지를 결정하는 변수 

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly,Category = "SkillInfo", ReplicatedUsing = "OnRep_CanSkill_Two")
	bool Can_Skill_Two = true;      //  스킬2가 사용 가능한 상태인지를 결정하는 변수

	UPROPERTY(EditAnywhere, Category = "ReferencesRequired|SkillInfo")
	bool ComboSkill = false;    // 스킬 키를 한번 더 눌러서 재사용해야하는 스킬이라면 true로 해서 스킬키를 더 누를수 있게 하는 변수

	UPROPERTY(EditAnywhere, Category = "ReferencesRequired|SkillInfo")
	bool ComboEnd = false;    // 콤보스킬이 완전히 끝났는지를 확인하는 변수

	UPROPERTY(Replicated)
	bool CanSkill1= true;

	UPROPERTY()
	bool CoolDown_On = false;   // 쿨타임을 실행할지를 확인하는 변수

public:
	UFUNCTION()
	void Skill_1();

	UFUNCTION()
	void Skill_2();


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ReferencesRequired|SkillInfo")
	float Skill_One_Cooldown = 5.0f;     // 스킬1의 쿨타임을 결정하는 변수

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ReferencesRequired|SkillInfo")
	float Skill_Two_Cooldown = 7.0f;      // 스킬2의 쿨타임을 결정하는 변수 

	UPROPERTY()
	float Skill_One_RemainingTime;

	UPROPERTY()
	float Skill_Two_RemainingTime;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ReferencesRequired|SkillInfo")
	float Skill_One_ActiveTime;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ReferencesRequired|SkillInfo")
	float Skill_Two_ActiveTime;

	UFUNCTION()
	void OnRep_CanSkill_One();

	UFUNCTION()
	void OnRep_CanSkill_Two();

	FTimerHandle Skill_One_CooldownTimer;

	FTimerHandle Skill_Two_CooldownTimer;


public:
	FSkillOneDele Skill_One_Change;
	
	FSkillTwoDele Skill_Two_Change;
};
