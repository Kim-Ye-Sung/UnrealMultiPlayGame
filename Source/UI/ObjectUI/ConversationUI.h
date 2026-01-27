// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ConversationUI.generated.h"

/**
 * 
 */
UCLASS()
class GOSOO_API UConversationUI : public UUserWidget
{
	GENERATED_BODY()
	
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* ConversationTextBlock;

	TArray<FText>CopyConversationTexts;   // 대사 목록

	int32 ConversationNumber = 0;     // 대사를 해야하는 순서

	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;

	UPROPERTY()
	TObjectPtr<class AItem> OwnerInteractObject;

public:
	void SetCopyConversationTexts(TArray<FText> ConversationArray);

	FORCEINLINE void SetOwnerInteractObject(AItem* OwnerObject) { OwnerInteractObject = OwnerObject; }

private:
	UFUNCTION()
	void ConversationStart();

};
