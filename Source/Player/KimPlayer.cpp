// Fill out your copyright notice in the Description page of Project Settings.


#include "KimPlayer.h"
#include "KimSkill.h"

AKimPlayer::AKimPlayer()
{
	static ConstructorHelpers::FObjectFinder<USkeletalMesh>KimPlayerMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/01_ReSource/CyberpunkPackVol1/Characters/Kim/Meshes/SK_Kim.SK_Kim'"));
	if (KimPlayerMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(KimPlayerMesh.Object);
	}

	SkillComponent = CreateDefaultSubobject<UKimSkill>(TEXT("KimSkill"));
	SkillComponent->SetupAttachment(RootComponent);
}


