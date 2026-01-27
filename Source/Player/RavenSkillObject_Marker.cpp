// Fill out your copyright notice in the Description page of Project Settings.


#include "RavenSkillObject_Marker.h"
#include "Components/SphereComponent.h"
#include "Components/BoxComponent.h"
#include "RavenPlayer.h"
#include "RavenSkill.h"
#include "Gosoo/Enemy/Base_Enemy.h"
#include "Engine/DamageEvents.h"


// Sets default values
ARavenSkillObject_Marker::ARavenSkillObject_Marker()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MarkerMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MarkerMesh"));
	RootComponent = MarkerMesh;
	MarkerMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	MarkerMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	MarkerMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
	//MarkerMesh->SetRelativeLocation(FVector(4.0f, 0.0f, -6.0f));   // 원래는 z축으로 -5 
	MarkerMesh->SetRelativeScale3D(FVector(0.1f, 0.1f, 0.1f));

	BaseComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BaseComponent"));
	BaseComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 50.0f));
	BaseComponent->SetupAttachment(RootComponent);
	BaseComponent->bHiddenInGame = false;
	BaseComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	BaseComponent->SetBoxExtent(FVector(20.0f, 50.0f, 35.0f));

	MarkerBoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("MarkerBoxComponent"));
	MarkerBoxComponent->SetRelativeLocation(FVector(-40.0f, 0.0f, 10.0f));
	MarkerBoxComponent->SetupAttachment(BaseComponent);
	MarkerBoxComponent->bHiddenInGame = false;
	MarkerBoxComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	MarkerBoxComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);  // 캐릭터와는 충돌하지 못하게 막음.

	MarkerBoxComponent->SetBoxExtent(FVector(20.0f, 50.0f, 35.0f));

	DamageRange = CreateDefaultSubobject<USphereComponent>(TEXT("DamageRange"));
	DamageRange->SetupAttachment(RootComponent);
	DamageRange->bHiddenInGame = false;
	DamageRange->SetSphereRadius(500.0f);
	DamageRange->SetCollisionEnabled(ECollisionEnabled::NoCollision);


}

// Called when the game starts or when spawned
void ARavenSkillObject_Marker::BeginPlay()
{
	Super::BeginPlay();

	OwnerPlayer = Cast<ARavenPlayer>(GetOwner());


	MarkerBoxComponent->OnComponentBeginOverlap.AddDynamic(this, &ARavenSkillObject_Marker::MarkerBeginOverlap);
	DamageRange->OnComponentBeginOverlap.AddDynamic(this, &ARavenSkillObject_Marker::ExploreBeginOverlap);
}

// Called every frame
void ARavenSkillObject_Marker::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ARavenSkillObject_Marker::MarkerBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Blue, TEXT("ProjectileOverlap!!!"));

	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, OtherActor->GetName());

	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, OverlappedComp->GetName());

	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, OtherComp->GetName());

	MarkerMesh->SetSimulatePhysics(false);

	MarkerMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	MarkerBoxComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	if (ABase_Enemy* EnemyActor = Cast<ABase_Enemy>(OtherActor))
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Blue, TEXT("ProjectileDamage!!!"));

		ExploreDamage();
	}
	else
	{
		DestroyMarker();
	}

	OwnerPlayer->SpawnSkillObject();

	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, TEXT("Destroy"));

	Destroy();
}

void ARavenSkillObject_Marker::DestroyMarker()
{
	if (OwnerPlayer)
	{
		if (URavenSkill* OwnerSkillComp = Cast<URavenSkill>(OwnerPlayer->GetPlayerSkill()))
		{
			FVector NowLocation = MarkerBoxComponent->GetComponentLocation();

			const float HalfHeight = OwnerPlayer->GetSimpleCollisionHalfHeight();

			NowLocation.Z = OwnerPlayer->GetActorLocation().Z + HalfHeight;

			GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, FString::Printf(TEXT("NowLocation.z : %f"), NowLocation.Z));


			OwnerSkillComp->SkillGoalLocation = NowLocation;


			OwnerSkillComp->HasGoalTransform = true;

			GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, TEXT("has Goal Location"));
		}
	}
}



void ARavenSkillObject_Marker::ExploreDamage()
{
	DamageRange->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void ARavenSkillObject_Marker::ExploreBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (ABase_Enemy* EnemyActor = Cast<ABase_Enemy>(OtherActor))
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, TEXT("EnemyHit"));

		FDamageEvent DamageEvent;
		EnemyActor->TakeDamage(100.0f, DamageEvent, OwnerPlayer->GetController(), this);  // 폭발에 맞은 적 HP 감소
	}
}

void ARavenSkillObject_Marker::MarkerMoveStart(FVector GoalVector)
{
	FRotator CurrentRotation = BaseComponent->GetComponentRotation();  // BaseComponent의 월드 회전값을 가져온다.

	BaseComponent->SetAbsolute(false, true, false);
	// true로하면 월드 좌표계를 사용한다는 뜻.(순서대로 Location, Rotation, Scale값이다)
	// 즉, 여기서는 Rotation값을 AbsoluteRotation으로 하여서 
	// 부모컴포넌트인 스태틱메쉬가 회전해도 박스컴포넌트는 회전을 안하겠다고 선언하는것.
	// 실제로 에디터상에서 Transform값을 보면 AbsoluteRotation으로 변경되어 있다.
	// 문제는 이러면 블루프린트상에서 조절했었던 제일 처음 설정했던 Relative회전값으로 되돌아가버리게 된다.

	BaseComponent->SetWorldRotation(CurrentRotation);  // 그래서 여기서 다시 가져왔었던 월드 회전값으로 변경해준다.

	MarkerMesh->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);

	MarkerMesh->SetSimulatePhysics(true);

	MarkerMesh->AddImpulse(GoalVector * ThrowSpeed);

	MarkerBoxComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);   // 마킹 판정 시작


	FTimerDelegate MarkerBreak;
	MarkerBreak.BindWeakLambda(this, [this]
		{
			GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, TEXT("TimerEnd"));

			MarkerMesh->SetSimulatePhysics(false);

			MarkerMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

			MarkerBoxComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

			ExploreDamage();

			OwnerPlayer->SpawnSkillObject();

			Destroy();
		}
	);

	GetWorldTimerManager().SetTimer(MarkerBreakTimer, MarkerBreak, BreakTime, false);
}

