// Fill out your copyright notice in the Description page of Project Settings.


#include "WhiteBlossomPlayer.h"
#include "WhiteBlossomSkill.h"

AWhiteBlossomPlayer::AWhiteBlossomPlayer()
{
	SkillComponent = CreateDefaultSubobject<UWhiteBlossomSkill>(TEXT("WhiteBlossomSkill"));

	const ConstructorHelpers::FObjectFinder<USkeletalMesh>BlossomMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/01_ReSource/CyberpunkPackVol1/Characters/WhiteBlossom/Meshes/SK_WhiteBlossom.SK_WhiteBlossom'"));
	if (BlossomMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(BlossomMesh.Object);
	}
}