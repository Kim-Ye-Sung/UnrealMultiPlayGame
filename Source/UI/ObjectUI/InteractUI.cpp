// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractUI.h"
#include "Components/TextBlock.h"

void UInteractUI::SetInteractableObjectText(FText InteractObjectText)
{
	GEngine->AddOnScreenDebugMessage(-1, 4.0f, FColor::Blue, TEXT("SetInteractText2222"));

	InteractableObjectText->SetText(InteractObjectText);
}
