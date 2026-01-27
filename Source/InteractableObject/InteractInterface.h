// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InteractInterface.generated.h"

/**
 *
 */
UINTERFACE(Blueprintable)
class GOSOO_API UInteractInterface : public UInterface
{
	GENERATED_BODY()
};

class GOSOO_API IInteractInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interaction", meta = (DisplayName = "Interact"))
	void InteractInterfaceFunction(class AGosooPlayer* PlayerCharacter);
};
