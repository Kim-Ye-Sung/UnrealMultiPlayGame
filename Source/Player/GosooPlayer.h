// Fill out your copyright notice in the Description page of Project Settings.

// Project_Gosoo에서 사용할 캐릭터 클래스의 헤더파일.


#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CharacterState.h"          // 캐릭터 enum 헤더파일 추가.
#include "../Gun/ABaseWeapon.h"
#include "GosooPlayer.generated.h"

UCLASS()
class GOSOO_API AGosooPlayer : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AGosooPlayer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USpringArmComponent> PlayerSpringArmComponent;    // 스프링암 컴포넌트 선언

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UCameraComponent> PlayerCameraComponent;    // 카메라 컴포넌트 선언

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ReferencesRequired|Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputMappingContext> DefaultMappingContext;     // 인풋매핑컨텍스트 선언

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ReferencesRequired|Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> MoveAction;     // 이동시 사용할 인풋액션 선언

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ReferencesRequired|Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> LookAction;    // 카메라 움직임 사용할 인풋액션 선언

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ReferencesRequired|Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> JumpAction;     // 점프시 사용할 인풋액션 선언

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ReferencesRequired|Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> CrouchAction;   // 앉기 기능 사용할 인풋액션 선언

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ReferencesRequired|Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> Dash_Run_Action; // 대시와 스프린트 기능 사용할 인풋액션 선언

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ReferencesRequired|Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> Interaction;    // 상호작용 기능 사용할 인풋액션 선언

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ReferencesRequired|Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> AttackAction;     // 기본공격 기능 사용할 인풋액션 선언

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ReferencesRequired|Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> MeleeAttack;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ReferencesRequired|Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> Skill_One;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ReferencesRequired|Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> Skill_Two;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ReferencesRequired|Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> EmotionAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ReferencesRequired|Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> Emotion1_SelectAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ReferencesRequired|Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> Emotion2_SelectAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ReferencesRequired|Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> Emotion3_SelectAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ReferencesRequired|Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> MapAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ReferencesRequired|Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> OptionOpenAction;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PlayerState", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UCharacterState> CharacterStateComponent;     // 캐릭터 상태 enum설정할 CharacterState 선언

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ReferencesRequired|AnimMontage", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> MeleeAttackMontage;     // 근접공격 몽타주 담는 변수 선언

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UBoxComponent> MeleeAttackHitBox;    // 근접공격 타격판정을 위한 박스 컴포넌트 선언.

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ReferencesRequired|AnimMontage", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAnimMontage> Emotion1Montage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ReferencesRequired|AnimMontage", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAnimMontage> Emotion2Montage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ReferencesRequired|AnimMontage", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAnimMontage> Emotion3Montage;


	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ReferencesRequired|Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> TestInput;     // hp테스트를 위한 것이니 나중에 지울것 



protected:
	void Basic_Move(const struct FInputActionValue& Value);     // MoveAction에 바인딩 할 함수 선언

	void Basic_Look(const struct FInputActionValue& Value);     // LookAction에 바인딩 할 함수 선언

	void Basic_Jump(const struct FInputActionValue& Value);     // JumpAction에 바인딩 할 함수 선언 
	// (점프도 기본적으로 캐릭터 무브먼트 컴포넌트쪽에서 처리가 가능하므로 서버RPC나 ReplicatedUsing등을 사용하지 않아도 된다.)

	void Basic_Crouch(const struct FInputActionValue& Value);  // 클라이언트쪽에서 먼저 RPC 호출 여부를 결정하는 함수.

	UFUNCTION(Server, Reliable)
	void Basic_Crouch_Server();    // 앉기 기능을 활성화시킬 서버RPC 선언

	void Basic_Dash(const struct FInputActionValue& Value);    // 클라이언트쪽에서 먼저 RPC 호출 여부를 결정하는 함수.

	UFUNCTION(Server, Reliable)
	void Basic_Dash_Server();     // Dash_Sprint_Action에 바인딩 할 함수 선언


	void Basic_Interaction_Trace(const struct FInputActionValue& Value);     // Interaction에 바인딩 할 함수 선언

	UFUNCTION(Server, Reliable)
	void Basic_Interaction(class AActor* InteractablActor);      // Basic_Interaction_Trace 함수에서 실행할 서버RPC 선언

	void Basic_MeleeAttack(const struct FInputActionValue& Value);

	UFUNCTION(Server, Reliable)
	void Basic_MeleeAttack_Server();

	UFUNCTION()
	virtual void Basic_Attack();        // 원래는 인자값으로 UAnimMontage를 넣어주려고했는데, RPC의 경우는 기본 변수타입들, 액터포인터, 구조체, UObject정도만 담을수있다.
												  // 따라서 UAnimMontage를 인자값으로 설정하고 cpp에서 정의하면 컴파일은 되나 실제로 언리얼에서 플레이하면 크래쉬나니 주의하자.




	UFUNCTION()
	void EmotionControl();

	UFUNCTION()
	void Emotion1();

	UFUNCTION(Server, UnReliable)
	void Emotion1_Server();

	UFUNCTION(NetMulticast, Unreliable)
	void Emotion1_NetMulticast();

	UFUNCTION()
	void Emotion2();

	UFUNCTION(Server, UnReliable)
	void Emotion2_Server();

	UFUNCTION(NetMulticast, Unreliable)
	void Emotion2_NetMulticast();

	UFUNCTION()
	void Emotion3();

	UFUNCTION(Server, Unreliable)
	void Emotion3_Server();

	UFUNCTION(NetMulticast, Unreliable)
	void Emotion3_NetMulticast();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Skills", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UPlayerSkill> SkillComponent;     // 스킬관리를 위한 스킬 컴포넌트 변수 선언


public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated)
	int32 JumpCount = 0;    // 점프 횟수를 저장하는 변수 선언

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated)
	bool IsLanding = true;   // 캐릭터가 지상에 있는지 확인하는 변수 선언

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated)   // 이 변수는 더블점프 실행이 끝나면 다시 JumpLoop 애니메이션 실행할수 있게끔 블루프린트에서 바꿀 변수임. 그래서 BlueprintReadWrite로 설정함.
	bool DoubleJumpEnd = false;    // 더블 점프 끝났는지 판단하는 변수 선언

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated)
	bool CanDash = true;         // 대시 기능을 쓸 수 있느냐를 판단하는 변수 선언 

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, ReplicatedUsing = "OnRep_CanMeleeAttack")
	bool CanMeleeAttack = true;

	UPROPERTY()
	bool IsMapUIOpen = false;

	UFUNCTION()
	void OnRep_CanMeleeAttack();

	UFUNCTION()
	void MeleeAttackHitCheck();

	UFUNCTION()
	void MeleeHitBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void MeleeHitBoxEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UPROPERTY()
	TArray<TObjectPtr<class ABase_Enemy>> HitObjs;    // 공격시 충돌한 오브젝트들을 담는 배열




	UFUNCTION(BlueprintCallable)
	void SetDoubleJumpEnd();    // DoubleJumpEnd 변수를 바꾸는 함수 선언


public:
	virtual void GetLifetimeReplicatedProps(TArray <FLifetimeProperty>& OutLifetimeProps) const override;


	UPROPERTY()
	TObjectPtr<class UAnimInstance> PlayerAnimInstance;

	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SeverComponent", meta = (AllowPrivateAccess = "true"))
	//TObjectPtr<class UServerComponent> ServerComponent;

	//UPROPERTY(ReplicatedUsing = "OnRep_GetItem")
	//bool GetItem = false;

	UPROPERTY()
	TObjectPtr<class AActor> InteractableActor = nullptr;    // 상호작용 가능한 물체가 있느냐를 판단하는 변수.

	FORCEINLINE void SetInteractableActor(AActor* InterActor) { InteractableActor = InterActor; }     // 상호작용 가능한 물체를 저장하는 변수.

	//UFUNCTION()
	//void OnRep_GetItem();


	//---------------------------------------------------//
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stat", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UPlayerStat> Stat;    // 캐릭터 스탯 관련 컴포넌트를 선언


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
	TSubclassOf <class AABaseWeapon> Weapon1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon", meta = (AllowPrivateAccess = "true"), Replicated)
	TObjectPtr <class AABaseWeapon> Weapon2;

	UFUNCTION()
	void SetWeapon2();


	UPROPERTY(EditAnywhere, Category = "ReferencesRequired|PlayerUI")
	TSubclassOf<class UPlayerUI> PersonalPlayerUIClass  = nullptr;    // 플레이어UI를 담을 클래스 변수

	UPROPERTY(VisibleAnywhere, Category = "PlayerUI")
	TObjectPtr<UPlayerUI> PersonalPlayerUI = nullptr;    // UI가 생성되고 난 것을 담을 UI 오브젝트 변수

	UPROPERTY(EditAnywhere, Category = "ReferencesRequired|PlayerUI")
	TSubclassOf<class UMapUI> PersonalMapUIClass = nullptr;

	UPROPERTY()
	TObjectPtr<UMapUI> PersonalMapUI = nullptr;

	UPROPERTY(EditAnywhere, Category = "ReferencesRequired|PlayerUI")
	TSubclassOf<class UOptionSelectUI> PersonalOptionSelectUIClass = nullptr;

	UPROPERTY()
	TObjectPtr<UOptionSelectUI> PersonalOptionSelectUI = nullptr;

	UFUNCTION()
	void MapFunction();

	UFUNCTION()
	void OptionSelectOpen();

	FORCEINLINE bool GetIsMapUIOpen() const { return IsMapUIOpen; }

	UFUNCTION(Server, Reliable)    // 테스트 용도로 놔둔거니 나중에 지울것
	void TestAction();


	// --------------------------------------- 인풋 액션들 얻어오는 함수들. --------------------------------------- //

	UFUNCTION()
	FORCEINLINE UInputMappingContext* GetPlayerMappingContext() const { return DefaultMappingContext; }

	UFUNCTION()
	FORCEINLINE UInputAction* GetPlayerAttackAction() const { return AttackAction; }

	UFUNCTION()
	FORCEINLINE UInputAction* GetPlayerJumpAction() const { return JumpAction; }

	UFUNCTION()
	FORCEINLINE UInputAction* GetPlayerCrouchAction() const { return CrouchAction; }

	UFUNCTION()
	FORCEINLINE UInputAction* GetPlayerDashRunAction() const { return Dash_Run_Action; }

	UFUNCTION()
	FORCEINLINE UInputAction* GetPlayerInteractionAction() const { return Interaction; }

	UFUNCTION()
	FORCEINLINE UInputAction* GetPlayerMeleeAttackAction() const { return MeleeAttack; }

	UFUNCTION()
	FORCEINLINE UPlayerStat* GetPlayerStat() const { return Stat; }

	//----------------------------------------------- 인풋 액션들 얻어오는 함수들 끝. ----------------------------------------------- //

	UFUNCTION(BlueprintCallable)
	FORCEINLINE UCameraComponent* GetPlayerCameraComponent() const { return PlayerCameraComponent; }

	UFUNCTION()
	FORCEINLINE UPlayerSkill* GetPlayerSkill() const { return SkillComponent; }


protected:
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;



	
	// ------------------------------------------미니맵을 위한 변수들 생성 ----------------------------------------------------//
private:
	UPROPERTY(VisibleAnywhere,Category = "Minimap")
	TObjectPtr<USpringArmComponent>MinimapSpringArmComponent;  // 캐릭터 위치를 미니맵에 띄우기 위한 스프링암 컴포넌트

	UPROPERTY(VisibleAnywhere, Category = "Minimap")
	TObjectPtr<class USceneCaptureComponent2D>MinimapCameraComponent;  // 캐릭터 위치를 미니맵에 띄우기 위한 씬캡쳐 카메라 컴포넌트

	UPROPERTY()
	TObjectPtr<class UTextureRenderTarget2D> MinimapRenderTarget;    // 캐릭터의 위치를 찍기 위한 텍스쳐 변수. 


protected:
	FTimerHandle DashEndTimer;  // 대시 애니메이션이 끝났을때 캐릭터의 상태를 변경해주기 위한 타이머.
	
	FTimerHandle DashCooldownTimer;   // 대시기능의 쿨타임 시간을 계산하는 타이머.

	FTimerHandle MeleeAttackCooldownTimer;   // 근접공격의 쿨타임 시간을 계산하는 타이머.

	FTimerHandle MeleeAttackHitCheckTimer;   // 근접공격의 피격 판정을 계산하는 타이머.
};

