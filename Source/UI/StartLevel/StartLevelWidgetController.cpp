// Fill out your copyright notice in the Description page of Project Settings.


#include "Gosoo/UI/StartLevel/StartLevelWidgetController.h"
#include "Gosoo/UI/StartLevel/StartLevelWidget.h"
#include "Components/Button.h" 
#include "GosooGlobal.h"
#include "NetworkInitializer.h"
#include "GosooGameInstance/Subsystem/GosooNetSubsystem.h"
#include "StartLevel/RoomAccessHelper.h"

// Sets default values
AStartLevelWidgetController::AStartLevelWidgetController()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AStartLevelWidgetController::BeginPlay()
{
	Super::BeginPlay();
	
	//임시코드
	auto netSubsystem = GetGameInstance()->GetSubsystem<UGosooNetSubsystem>();
	mInitializer = netSubsystem->GetNetInitializerPtr();
	mInitializer->AddFuncOnInitComplete(this, TEXT("CreateStartLevelWidget"));

	//5초뒤 초기화
	GetWorld()->GetTimerManager().SetTimer(
		mNetInitTimerHandle,
		mInitializer,
		&UNetworkInitializer::Init,
		10.0f,
		false
	);

}

// Called every frame
void AStartLevelWidgetController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AStartLevelWidgetController::MakeRoom()
{
	if (!mRoomAccessHelperPtr) return;
	mRoomAccessHelperPtr->MakeRoom();
}

void AStartLevelWidgetController::JoinRoom()
{
	if (!mRoomAccessHelperPtr) return;
	mRoomAccessHelperPtr->JoinRoom();
}

void AStartLevelWidgetController::CreateStartLevelWidget()
{
	if (!mStartLevelWidgetClass) return;
	mStartLevelWidgetPtr = CreateWidget<UStartLevelWidget>(GetWorld(), mStartLevelWidgetClass);

	if (!mStartLevelWidgetPtr) return;
	mStartLevelWidgetPtr->AddToViewport();

	mStartLevelWidgetPtr->Make_Button->OnClicked.AddDynamic(this, &AStartLevelWidgetController::MakeRoom);
	mStartLevelWidgetPtr->Join_Button->OnClicked.AddDynamic(this, &AStartLevelWidgetController::JoinRoom);

	if (!mRoomAccessHelperClass) return;
	mRoomAccessHelperPtr = GetWorld()->SpawnActor<ARoomAccessHelper>(mRoomAccessHelperClass);
}

