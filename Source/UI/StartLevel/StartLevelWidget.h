// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "StartLevelWidget.generated.h"

/**
 * 
 */
UCLASS()
class GOSOO_API UStartLevelWidget : public UUserWidget
{
	GENERATED_BODY()
	
	
public:
	UPROPERTY(meta = (BindWidget))
	class UButton* Make_Button;
	UPROPERTY(meta = (BindWidget))
	class UButton* Join_Button;
};
