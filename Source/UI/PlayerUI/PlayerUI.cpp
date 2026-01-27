// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerUI.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"
#include "Components/Image.h"
#include "Components/Overlay.h"


void UPlayerUI::Set_HP_Text(float CurrentHP, float MaxHP)
{
	CurrentPlayer_HP->SetText(FText::FromString(FString::Printf(TEXT("%d/%d"),(int)CurrentHP, (int)MaxHP)));

	HP_ProgressBar->SetPercent(CurrentHP / MaxHP);
}

void UPlayerUI::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	WatchEmotionScreen();    // UI가 Add to viewport될때 감정표현 선택창이 꺼지고 시작하도록 한다.
}

//void UPlayerUI::NativeConstruct()
//{
//	WatchEmotionScreen();    // UI가 Add to viewport될때 감정표현 선택창이 꺼지고 시작하도록 한다.
//}

void UPlayerUI::SetMinimapImage(UTextureRenderTarget2D* PlayerMinimapRenderTarget)
{
	if (MinimapMaterial && PlayerMinimapRenderTarget)
	{
		MinimapMID = UMaterialInstanceDynamic::Create(MinimapMaterial, this);  // 각 캐릭터별로 뼈대 머티리얼을 기반으로 머티리얼 인스턴스를 생성.

		MinimapMID->SetTextureParameterValue(TEXT("MinimapRenderTarget"), PlayerMinimapRenderTarget);  // 머티리얼 인스턴스로 파라미터 변경.
		// MinimapMaterial에 "MinimapRenderTarget"이랑 똑같은 ParameterName으로 설정된 TextureSampleParameter2D노드가 있어야한다. 
		// 그리고 이 코드는 그 파라미터에 텍스쳐를 변경해주는것. 

		Minimap_Image->SetBrushFromMaterial(MinimapMID);    // 이미지에 미니맵 머티리얼 인스턴스 할당.
	}
}

void UPlayerUI::SetFirstSkillProgress(float RemainingTime, float Cooldown)
{
	FirstSkill->SetPercent(RemainingTime / Cooldown);

	if (RemainingTime >= Cooldown)
	{
		FirstSkillRemainingText->SetRenderOpacity(0.0f);
	}
	else
	{
		FirstSkillRemainingText->SetRenderOpacity(1.0f);
		FirstSkillRemainingText->SetText(FText::FromString(FString::Printf(TEXT("%.1f s"), Cooldown - RemainingTime)));
	}
}

void UPlayerUI::SetSecondSkillProgress(float RemainingTime, float Cooldown)
{
	SecondSkill->SetPercent(RemainingTime / Cooldown);
	
	if (RemainingTime >= Cooldown)
	{
		SecondSkillRemainingText->SetRenderOpacity(0.0f);
	}
	else
	{	
		SecondSkillRemainingText->SetRenderOpacity(1.0f);

		SecondSkillRemainingText->SetText(FText::FromString(FString::Printf(TEXT("%.1f s"), Cooldown - RemainingTime)));   // 남은 쿨타임 시간을 표시한다.
	}
}

void UPlayerUI::WatchEmotionScreen()
{	
	if (!IsEmotionOverlayOpen) // IsEmotionOverlayOpen이 false면 
	{
		EmotionOverlay->SetRenderOpacity(1.0f);     // 감정표현 선택창이 보이게 한다.

		IsEmotionOverlayOpen = !IsEmotionOverlayOpen;   // 함수를 실행할때마다 bool값을 반대로 바꾸어서 감정표현 선택창의 투명도 여부를 결정한다.
	}   
	else        
	{
		EmotionOverlay->SetRenderOpacity(0.0f);   // 감정표현 선택창이 보이지 않게 한다.

		IsEmotionOverlayOpen = !IsEmotionOverlayOpen;
	}
}