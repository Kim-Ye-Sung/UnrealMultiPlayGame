// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CharacterState.generated.h"


UENUM(BlueprintType, meta = (ScriptName = "GosooPlayerState"))
enum class EPlayerState : uint8  // uint8로 지정했으므로 256개까지 열거 가능.
{
	Move,
	Crouch,
	Run,
	Dash,
	Jump,
	Climb,
	Attack,
};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class GOSOO_API UCharacterState : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UCharacterState();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PlayerState", ReplicatedUsing = "OnRep_CurrentState")
	EPlayerState CurrentState = EPlayerState::Move;       // 현재 상태를 저장하는 변수 선언. 기본적으로 시작할때는 Move상태로 시작.


	virtual void GetLifetimeReplicatedProps(TArray <FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	UPROPERTY()
	TObjectPtr<class AGosooPlayer> OwnerCharacter = nullptr;

public:

	FORCEINLINE EPlayerState GetCurrentState() const { return CurrentState; }     // 현재 상태를 알아내는 Get함수 선언

	UFUNCTION(Server, Reliable)
	void SetCurrentState(EPlayerState NewState);   // 플레이어의 상태를 변경하는 함수 선언

	UFUNCTION()
	void OnRep_CurrentState();

};
