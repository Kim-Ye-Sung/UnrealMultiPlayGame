// Fill out your copyright notice in the Description page of Project Settings.


#include "MapUI.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Components/Image.h"

void UMapUI::NativeConstruct()
{
	Super::NativeConstruct();
}

void UMapUI::SetMapImage(UTextureRenderTarget2D* MapRenderTarget)
{
	if (MapMaterial && MapRenderTarget)
	{
		MapMaterialDynamic = UMaterialInstanceDynamic::Create(MapMaterial, this);

		MapMaterialDynamic->SetTextureParameterValue(TEXT("MapRenderTarget"), MapRenderTarget);

		MapImage->SetBrushFromMaterial(MapMaterialDynamic);
	}
}

