// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerStat.h"
#include "Net/UnrealNetwork.h"
#include "GosooPlayer.h"
#include "Gosoo/UI/PlayerUI/PlayerUI.h"

// Sets default values for this component's properties
UPlayerStat::UPlayerStat()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	SetIsReplicatedByDefault(true);
	// ...
}


// Called when the game starts
void UPlayerStat::BeginPlay()
{
	Super::BeginPlay();

	// ...
	OwnerCharacter = Cast<AGosooPlayer>(GetOwner());
	
}


// Called every frame
void UPlayerStat::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UPlayerStat::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(UPlayerStat, HP);
}

void UPlayerStat::OnRep_HP()
{
	if (OwnerCharacter == nullptr)
	{
		return;
	}

	HP_Change_Function.Broadcast(HP, MaxHP);    // HP 바뀐것들에 대해서 델리게이트 함수들 실행

	if(OwnerCharacter->GetIsMapUIOpen() == true)    // 데미지를 입었을때, 지도가 켜져있는 상태라면
	{
		OwnerCharacter->MapFunction();     // 지도 끄기
	}
}