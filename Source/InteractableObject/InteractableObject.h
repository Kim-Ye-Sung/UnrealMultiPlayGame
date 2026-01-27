// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
//#include "InteractInterface.h"                 // 인터페이스 헤더 파일 선언 
#include "Gosoo/Player/GosooPlayer.h"
#include "Components/BoxComponent.h"
#include "InteractableObject.generated.h"

UCLASS()
class GOSOO_API AInteractableObject : public AActor 
	//public IInteractInterface    // 인터페이스를 사용하기 위해 상속받아줌.
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AInteractableObject();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, Category = "SceneComponent")
	TObjectPtr<class USceneComponent>SceneComponent;

protected:
	UPROPERTY(VisibleAnywhere, Category = "BoxComponent")
	TObjectPtr<class UBoxComponent> RangeBox;

	UFUNCTION()
	virtual void RangeBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	virtual void RangeBoxEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
	UPROPERTY(EditAnywhere, Category = "ReferencesRequired|InteractUI")
	FText InteractText;

	UPROPERTY(EditAnywhere, Category = "ReferencesRequired|Conversation")
	TArray<FText> ConversationTexts;

public:
	//UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interaction", meta = (DisplayName = "Interact"))
	//void InteractInterfaceFunction(class AGosooPlayer* PlayerCharacter);
};
