// Fill out your copyright notice in the Description page of Project Settings.


#include "RavenPlayer.h"
#include "RavenSkill.h"
#include "RavenSkillObject_Marker.h"

ARavenPlayer::ARavenPlayer()
{
	SkillComponent = CreateDefaultSubobject<URavenSkill>(TEXT("RavenSkill"));
}

void ARavenPlayer::BeginPlay()
{
	Super::BeginPlay();

	SpawnSkillObject();
}

void ARavenPlayer::DetachSkillObject()
{
	SpawnedSkillObject->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);   // 몽타주에서 노티파이로 이 함수를 실행.

	SpawnedSkillObject->MarkerMoveStart(GetMesh()->GetRightVector());

	//GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Black, TEXT("AAAAAAAA"));
}

void ARavenPlayer::SpawnSkillObject()
{
	if (SkillObject)
	{
		FActorSpawnParameters Params;
		Params.Owner = this;   // 스폰된 오브젝트의 오너는 레이브플레이어라고 지정함.

		SpawnedSkillObject = GetWorld()->SpawnActor<ARavenSkillObject_Marker>(SkillObject, GetActorTransform(), Params);  // 스킬 오브젝트 소환
		SpawnedSkillObject->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("hand_rSocket"));  // 오른손에 붙임.
		SpawnedSkillObject->SetActorRelativeLocation(FVector(-14.0f, 7.0f, 3.0f));    // 오른손 기준
		SpawnedSkillObject->SetActorRelativeRotation(FRotator(-51.0f, -99.0f, 94.0f));  // 오른손 기준
	}
}
