// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GosooPlayer.h"
#include "EvelynnPlayer.generated.h"

/**
 * 
 */
UCLASS()
class GOSOO_API AEvelynnPlayer : public AGosooPlayer
{
	GENERATED_BODY()

	AEvelynnPlayer();

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "EvelynnWeapon", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStaticMeshComponent> EvelynnWeapon;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "EvelynnWeapon", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UBoxComponent>WeaponHitBox;

	void Basic_Attack() override;

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void WeaponHitBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintCallable)
	void WeaponHitFinish();
};
