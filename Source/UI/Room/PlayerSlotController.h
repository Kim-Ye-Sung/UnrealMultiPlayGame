// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PlayerSlotController.generated.h"

UCLASS()
class GOSOO_API APlayerSlotController : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APlayerSlotController();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void Init(class UCanvasPanel* _canvasPanel);

public:
	UPROPERTY(EditAnywhere)
	int slotCount = 4;
	UPROPERTY(EditAnywhere)
	FVector2D firstSlotPos = FVector2D(-100, -20);

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	TSubclassOf<UUserWidget> mPlayerSlotWidgetClass;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "UI")
	TArray<TObjectPtr<class UPlayerSlotWidget>> mPlayerSlotWidgetArray;
	
};
