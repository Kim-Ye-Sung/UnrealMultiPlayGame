// Fill out your copyright notice in the Description page of Project Settings.


#include "Gosoo/UI/Room/PlayerSlotController.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Gosoo/UI/Room/PlayerSlotWidget.h"


// Sets default values
APlayerSlotController::APlayerSlotController()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APlayerSlotController::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APlayerSlotController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APlayerSlotController::Init(UCanvasPanel* _canvasPanel)
{
	if (!mPlayerSlotWidgetClass) return;

	for (int i = 0; i < slotCount; i++)
	{
		auto playerSlotWidget = CreateWidget<UPlayerSlotWidget>(GetWorld(), mPlayerSlotWidgetClass);
		if (!playerSlotWidget) return;

		mPlayerSlotWidgetArray.Add(playerSlotWidget);

		auto canvasPanelSlot = _canvasPanel->AddChildToCanvas(mPlayerSlotWidgetArray[i]);
		canvasPanelSlot->SetPosition(FVector2D(firstSlotPos.X + i * 250, firstSlotPos.Y));
		
		mPlayerSlotWidgetArray[i]->PlayerInvite_ScaleBox->SetVisibility(ESlateVisibility::Collapsed);
	}
}

