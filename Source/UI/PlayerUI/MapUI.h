// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MapUI.generated.h"

/**
 * 
 */
UCLASS()
class GOSOO_API UMapUI : public UUserWidget
{
	GENERATED_BODY()


	UPROPERTY(meta = (BindWidget))
	class UImage* MapImage;

	
	UPROPERTY(EditAnywhere, Category = "ReferencesRequired|Material")
	TObjectPtr<class UMaterialInterface> MapMaterial;                 // 전체 맵 기능을 위한 머티리얼

	UPROPERTY()
	TObjectPtr<class UMaterialInstanceDynamic> MapMaterialDynamic;    // UI상에 표시하기 위한 머티리얼인스턴스다이나믹


	virtual void NativeConstruct() override;  // BeginPlay 대신 사용할 함수

	UFUNCTION()
	void SetMapImage(class UTextureRenderTarget2D* MapRenderTarget);

	
	
};
