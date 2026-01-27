// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractableObject.h"

// ------------------------------- 정리 --------------------------- 
// 나는 상호작용 시작을 구현한것
// 구현한것을 바탕으로 파생시킬것
// 예를들어 발전기랑 상호작용하면 다른 기믹이 생김
// 
//


// Sets default values
AInteractableObject::AInteractableObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	RootComponent = SceneComponent;

	RangeBox = CreateDefaultSubobject<UBoxComponent>(TEXT("RangeBox"));
	RangeBox->SetupAttachment(RootComponent);
	RangeBox->SetBoxExtent(FVector(300.0f, 300.0f, 300.0f));

	RangeBox->bHiddenInGame = false;

	bReplicates = false;


}

// Called when the game starts or when spawned
void AInteractableObject::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AInteractableObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

//void AInteractableObject::InteractInterfaceFunction_Implementation(AGosooPlayer* PlayerCharacter)
//{
//	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, TEXT("Treasure Box"));
//}

void AInteractableObject::RangeBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

}

void AInteractableObject::RangeBoxEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	//if (AGosooPlayer* OverlapPlayer = Cast<AGosooPlayer>(OtherActor))
	//{
	//	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Emerald, TEXT("uuuuuuuu"));
	//}
}

