// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InteractUI.generated.h"

/**
 * 
 */
UCLASS()
class GOSOO_API UInteractUI : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* InteractableObjectText;

	UFUNCTION()
	void SetInteractableObjectText(FText InteractObjectText);
	
};
