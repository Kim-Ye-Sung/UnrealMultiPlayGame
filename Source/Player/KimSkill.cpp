// Fill out your copyright notice in the Description page of Project Settings.


#include "KimSkill.h"
#include "Components/SphereComponent.h"

UKimSkill::UKimSkill()
{
	SandevistanDomain = CreateDefaultSubobject<USphereComponent>(TEXT("SandevistanDomain"));  // 생성자에서 스킬관련 영역을 생성
	SandevistanDomain->SetSphereRadius(0.0f);    // 영역은 스킬 발동시 커져야하므로 여기서는 0으로 초기화
	SandevistanDomain->bHiddenInGame = false;

	Skill_One_Cooldown = 15.0f;   // 기획서상으로는 35초라고 되어있음. 추후 35초로 수정요망.

	Skill_One_ActiveTime = 5.0f;

	SandevistanEffect = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SandevistanEffect"));
	SandevistanEffect->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SandevistanEffect->SetRelativeScale3D(FVector(0.0f, 0.0f, 0.0f));
}


void UKimSkill::BeginPlay()
{
	Super::BeginPlay();

	// 생성자에서 붙이지 못했던 영역을 시작할때 붙임.
	SandevistanDomain->AttachToComponent(this, FAttachmentTransformRules::KeepRelativeTransform);  
	SandevistanDomain->SetRelativeLocation(FVector(0.0f, 0.0f, -95.0f));   

	SandevistanEffect->AttachToComponent(SandevistanDomain, FAttachmentTransformRules::KeepRelativeTransform);
}

void UKimSkill::Skill_1_Excute()
{
	Steps = FMath::Max(1, FMath::RoundToInt(GrowDuration / GrowInterval));
	RadiusStep = TargetDomainSize / Steps;

	FTimerDelegate SandevistanDomainReduction;
	SandevistanDomainReduction.BindWeakLambda(this, [this]()
		{
			switch (CurrentKimSkillState)
			{
			case EKimSkillState::Expanding:
			{
				// 스킬 영역을 현재 캐릭터의 위치로부터 생성되게끔하기 위해 현재 월드 좌표로부터 계산하여 떨어지게 컴포넌트를 떼어낸다.
				SandevistanDomain->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);

				CurrentDomainSize += RadiusStep;

				if (CurrentDomainSize >= TargetDomainSize)
				{
					CurrentDomainSize = TargetDomainSize;

					CurrentKimSkillState = EKimSkillState::Holding;
				}
				SandevistanDomain->SetSphereRadius(CurrentDomainSize);

				// 스피어컴포넌트에 자식컴포넌트로 있는 스태틱메시 컴포넌트의 스태틱메시가 언리얼엔진의 기본 스피어 메시이고,
				// 기본 스피어 메시가 반지름이 50cm이므로 다음과 같이 계산하여 크기를 정해준다.
				SandevistanEffect->SetRelativeScale3D(FVector(CurrentDomainSize/50.0f));  			
				break;
			}
			case EKimSkillState::Holding:
			{
				CurrentHoldingTime += GrowInterval;

				if (CurrentHoldingTime >= Skill_One_ActiveTime)
				{
					CurrentKimSkillState = EKimSkillState::Shrinking;
					
					CurrentHoldingTime = 0.0f;
				}
				break;
			}
			case EKimSkillState::Shrinking:
			{
				CurrentDomainSize -= RadiusStep;

				if (CurrentDomainSize <= 0.0f)
				{
					CurrentDomainSize = 0.0f;

					CurrentKimSkillState = EKimSkillState::Expanding;

					// 스킬 영역이 완전히 줄어들었다면 다음 스킬 생성시 다시 현재 캐릭터의 위치로부터 영역이 생성되게끔 하기 위해
					// 캐릭터의 위치로 붙여준다. 이때는 KeepRelativeTransform, KeepWorldTransform을 쓰든 상관은 없다. 
					// 어차피 SetRelativeLocation으로 캐릭터의 위치로 옮겨줄것이기 때문이다.
					SandevistanDomain->AttachToComponent(this, FAttachmentTransformRules::KeepRelativeTransform);
					SandevistanDomain->SetRelativeLocation(FVector(0.0f, 0.0f, -95.0f));

					GetWorld()->GetTimerManager().ClearTimer(SandevistanTimer);
				}
				SandevistanDomain->SetSphereRadius(CurrentDomainSize);

				SandevistanEffect->SetRelativeScale3D(FVector(CurrentDomainSize / 50.0f));

				break;
			}
			}
		}
	);


	GetWorld()->GetTimerManager().SetTimer(SandevistanTimer, SandevistanDomainReduction, GrowInterval, true);
}

void UKimSkill::Skill_2_Excute()
{
}
