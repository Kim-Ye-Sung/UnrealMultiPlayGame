// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "StartLevelWidgetController.generated.h"

UCLASS()
class GOSOO_API AStartLevelWidgetController : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AStartLevelWidgetController();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void MakeRoom();
	UFUNCTION()
	void JoinRoom();
private:
	UFUNCTION()
	void CreateStartLevelWidget();
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	TSubclassOf<UUserWidget> mStartLevelWidgetClass;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	TObjectPtr<class UStartLevelWidget> mStartLevelWidgetPtr;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class ARoomAccessHelper> mRoomAccessHelperClass;
	UPROPERTY(VisibleDefaultsOnly)
	class ARoomAccessHelper* mRoomAccessHelperPtr = nullptr;
private:
	//테스트를 위한 임시 코드 //

	class UNetworkInitializer* mInitializer = nullptr;
	FTimerHandle mNetInitTimerHandle;

};
