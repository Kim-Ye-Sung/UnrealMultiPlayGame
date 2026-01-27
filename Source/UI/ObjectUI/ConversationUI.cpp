// Fill out your copyright notice in the Description page of Project Settings.


#include "ConversationUI.h"
#include "Components/TextBlock.h"
#include "Gosoo/InteractableObject/Item.h"

FReply UConversationUI::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	ConversationStart();  // 마우스를 누르면 다음 대사 실행
	return FReply::Handled();
}

FReply UConversationUI::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	if (InKeyEvent.GetKey() == EKeys::B)    
	{
		ConversationStart();        // 다음 대사 실행
		return FReply::Handled();
	}

	return FReply::Unhandled();
}

void UConversationUI::SetCopyConversationTexts(TArray<FText> ConversationArray)
{
	if (ConversationArray.Num() == 0)
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Blue, TEXT("0 Conversation"));

		return;
	}

	CopyConversationTexts = ConversationArray;    // 상호작용 오브젝트로 부터 받은 텍스트를 저장

	ConversationStart();    // 텍스트에 넣어서 띄우기
}

void UConversationUI::ConversationStart()
{
	if (ConversationNumber >= CopyConversationTexts.Num())    // 배열의 텍스트 만큼 대사를 완료했으면
	{
		ConversationNumber = 0;    // 다시 대사를 처음으로 되돌리기 위해 변수 변경

		OwnerInteractObject->CloseInteractInputMode();    // 대화창 종료하고, 게임 할 수 있게끔 입력버튼 되돌리기.
	}

	ConversationTextBlock->SetText(CopyConversationTexts[ConversationNumber]);   // 배열의 순서에 맞게 대사를 설정

	ConversationNumber++;    // 대사 순서 변경
}
