// Fill out your copyright notice in the Description page of Project Settings.


#include "EvelynnPlayer.h"
#include "EvelynnSkill.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Gosoo/Enemy/Base_Enemy.h"
#include "Engine/DamageEvents.h"

AEvelynnPlayer::AEvelynnPlayer()
{
	static ConstructorHelpers::FObjectFinder<USkeletalMesh>EvelynnPlayerMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/01_ReSource/CyberpunkPackVol1/Characters/EvelynnV3/Meshes/SK_EvelynnV3.SK_EvelynnV3'"));
	if (EvelynnPlayerMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(EvelynnPlayerMesh.Object);
	}

	EvelynnWeapon = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("EvelynnWeapon"));
	EvelynnWeapon->SetupAttachment(GetMesh(), TEXT("neck_01Socket"));
	//EvelynnWeapon->SetupAttachment(GetMesh(), TEXT("hand_rSocket"));

	static ConstructorHelpers::FObjectFinder<UStaticMesh>EvelynnWeaponMesh(TEXT("/Script/Engine.StaticMesh'/Game/0_DEV/KYS/PlayerCharacter/Evelynn/SM_Peacemaker.SM_Peacemaker'"));
	if (EvelynnWeaponMesh.Succeeded())
	{
		EvelynnWeapon->SetStaticMesh(EvelynnWeaponMesh.Object);
	}

	EvelynnWeapon->SetRelativeLocation(FVector(11.0f, -6.0f, 5.0f));            // 이것들은 칼을 등에 메고 있을 때 위치
	EvelynnWeapon->SetRelativeRotation(FRotator(-70.0f, -13.0f, 194.0f));
	EvelynnWeapon->SetCollisionProfileName(TEXT("NoCollision"));

	SkillComponent = CreateDefaultSubobject<UEvelynnSkill>(TEXT("EvelynnSkill"));   // 이블린 전용 스킬 컴포넌트 생성

	WeaponHitBox = CreateDefaultSubobject<UBoxComponent>(TEXT("WeaponHitBox"));     // 무기의 타격판정을 위한 히트박스 생성
	WeaponHitBox->bHiddenInGame = false;
	WeaponHitBox->SetupAttachment(EvelynnWeapon, TEXT("HitBoxSocket"));
	WeaponHitBox->SetBoxExtent(FVector(8.0f, 10.0f, 48.0f));
	WeaponHitBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);

}

void AEvelynnPlayer::BeginPlay()
{
	Super::BeginPlay();

	WeaponHitBox->OnComponentBeginOverlap.AddDynamic(this, &AEvelynnPlayer::WeaponHitBoxBeginOverlap);    
}


void AEvelynnPlayer::Basic_Attack()
{
	UEvelynnSkill* EveSkill = Cast<UEvelynnSkill>(SkillComponent);

	if (EveSkill->GetCurrentEvelynnSkillState() == EEvelynnSkillState::WeaponUsing)   // 칼 무기를 쥐고 있을때에만 공격을 칼로 함.
	{
		WeaponHitBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);   // 칼 공격 판정 시작(공격 판정 끝내는것은 ABP에서 구현함.)
		 
		EveSkill->SlashAction();   // 칼 공격 실행.
	}
}

void AEvelynnPlayer::WeaponHitBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (ABase_Enemy* HitObj = Cast<ABase_Enemy>(OtherActor))   // 칼에 맞은 물체가 적이라면
	{
		if (HitObjs.Contains(HitObj))    // 칼을 한번 휘두를때마다 따로 계산한다. 이미 데미지를 한번 줬던 적이라면 다시 데미지를 주지 않음.
		{
			return;
		}

		HitObjs.Add(HitObj);   // 다시 데미지를 주지 않기 위해 배열에 담는다.

		FDamageEvent DamageEvent;
		HitObj->TakeDamage(100.0f, DamageEvent, GetController(), this);
	}
}

void AEvelynnPlayer::WeaponHitFinish()   // 몽타주의 노티파이로 실행할 함수 정의   
{
	HitObjs.Empty();   // 칼을 한번 휘두르고 나면 다시 적들한테 데미지를 줄 수 있게끔 배열을 비운다.
}

