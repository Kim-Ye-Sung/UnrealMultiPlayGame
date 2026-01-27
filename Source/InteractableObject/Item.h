// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractableObject.h"
#include "InteractInterface.h"
#include "Item.generated.h"

/**
 * 
 */
UCLASS()
class GOSOO_API AItem : public AInteractableObject, public IInteractInterface
{
	GENERATED_BODY()
	
public:
	AItem();

private:
	UPROPERTY(VisibleAnywhere, Category = "StaticMeshComponent")
	TObjectPtr<UStaticMeshComponent> MeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "WidgetComponent", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UWidgetComponent>TextWidgetComponent;

	UPROPERTY(EditAnywhere, Category = "ReferencesRequired|ConversationUI")
	TSubclassOf<class UConversationUI> ConversationUIClass;

	UPROPERTY(VisibleAnywhere, Category = "ConversationUI")
	TObjectPtr<UConversationUI> ConversationUIObject;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class AGosooPlayer> InteractPlayer;
	
	UFUNCTION()
	void OpenInteractInputMode();


protected:
	virtual void BeginPlay() override;

	void RangeBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
	
	void RangeBoxEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interaction", meta = (DisplayName = "Interact"))
	void InteractInterfaceFunction(class AGosooPlayer* PlayerCharacter);

	UFUNCTION()
	void CloseInteractInputMode();
};
