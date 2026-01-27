// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RavenSkillObject_Marker.generated.h"

UCLASS()
class GOSOO_API ARavenSkillObject_Marker : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARavenSkillObject_Marker();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:

	UPROPERTY(EditAnywhere)
	TObjectPtr<class UBoxComponent> BaseComponent;

	UPROPERTY(EditAnywhere)
	TObjectPtr<class UStaticMeshComponent> MarkerMesh;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UBoxComponent>MarkerBoxComponent;

	UPROPERTY(EditAnywhere)
	TObjectPtr<class USphereComponent>DamageRange;

	UPROPERTY(EditAnywhere, Category = "ReferencesRequired|ObjectInfo")
	float ThrowSpeed = 300.0f;

	UFUNCTION()
	void MarkerBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void DestroyMarker();

	UPROPERTY(EditAnywhere, Category = "ReferencesRequired|ObjectInfo")
	float BreakTime = 3.0f;   // 오브젝트를 굴린 후 충돌하지 않음에도 부서지는 타이머 시간.

	FTimerHandle MarkerBreakTimer;

	UFUNCTION()
	void ExploreDamage();

	//UPROPERTY()
	//TArray<TObjectPtr<class ABase_Enemy>> Enemys;

	UPROPERTY()
	TObjectPtr<class ARavenPlayer> OwnerPlayer;

	UFUNCTION()
	void ExploreBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:
	UFUNCTION()
	void MarkerMoveStart(FVector GoalVector);
};
