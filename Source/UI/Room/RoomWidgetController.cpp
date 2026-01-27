// Fill out your copyright notice in the Description page of Project Settings.


#include "Gosoo/UI/Room/RoomWidgetController.h"
#include "Gosoo/UI/Room/PlayerSlotController.h"
#include "Components/CanvasPanel.h"
#include "Gosoo/UI/Room/RoomWidget.h"

// Sets default values
ARoomWidgetController::ARoomWidgetController()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ARoomWidgetController::BeginPlay()
{
	Super::BeginPlay();
	
	if (!mRoomWidgetClass) return;
	mRoomWidgetPtr = CreateWidget<URoomWidget>(GetWorld(), mRoomWidgetClass);
	
	if (!mRoomWidgetPtr) return;
	mRoomWidgetPtr->AddToViewport();

	if (!mPlayerSlotControllerClass) return;
	mPlayerSlotController = GetWorld()->SpawnActor<APlayerSlotController>(mPlayerSlotControllerClass);

	if (!mPlayerSlotController) return;
	mPlayerSlotController->Init(mRoomWidgetPtr->Room_CanvasPanel);
}

// Called every frame
void ARoomWidgetController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


}

