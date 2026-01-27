// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ScaleBox.h"
#include "PlayerSlotWidget.generated.h"

/**
 * 
 */
UCLASS()
class GOSOO_API UPlayerSlotWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(meta = (BindWidget))
	class UScaleBox* PlayerInvite_ScaleBox;
	
};
