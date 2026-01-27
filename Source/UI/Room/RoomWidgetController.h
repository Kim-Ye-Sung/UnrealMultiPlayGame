// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RoomWidgetController.generated.h"

UCLASS()
class GOSOO_API ARoomWidgetController : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARoomWidgetController();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	TSubclassOf<UUserWidget> mRoomWidgetClass;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "UI")
	TObjectPtr<class URoomWidget> mRoomWidgetPtr;

	//플레이어 슬롯
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	TSubclassOf<AActor> mPlayerSlotControllerClass;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "UI")
	TObjectPtr<class APlayerSlotController> mPlayerSlotController;
};
