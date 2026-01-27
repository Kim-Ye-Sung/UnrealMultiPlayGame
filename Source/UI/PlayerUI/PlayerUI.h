// Fill out your copyright notice in the Description page of Project Settings.

// 각 플레이어별로 화면에 보이는 체력이라던가 UI를 나타내는 위젯 클래스

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerUI.generated.h"

/**
 * 
 */

UCLASS()
class GOSOO_API UPlayerUI : public UUserWidget
{
	GENERATED_BODY()
	
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* CurrentPlayer_HP;

	UPROPERTY(meta = (BindWidget))
	class UProgressBar* HP_ProgressBar;

	UPROPERTY(meta = (BindWidget))
	class UImage* Minimap_Image;

	UPROPERTY(meta = (BindWidget))
	UProgressBar* FirstSkill;

	UPROPERTY(meta = (BindWidget))
	UProgressBar* SecondSkill;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* FirstSkillRemainingText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* SecondSkillRemainingText;

	UPROPERTY(meta = (BindWidget))
	class UOverlay* EmotionOverlay;

	UPROPERTY()
	bool IsEmotionOverlayOpen = true;    // 감정표현 선택창을 켜냐마냐를 결정하는 변수.

	UPROPERTY(EditAnywhere, Category = "ReferencesRequired|Minimap")
	TObjectPtr<class UMaterialInterface> MinimapMaterial;  // UI에 띄우기 위한 미니맵 머티리얼의 뼈대.

	UPROPERTY()
	TObjectPtr<class UMaterialInstanceDynamic> MinimapMID;  // 할당받은 미니맵 머티리얼을 기반으로 각 캐릭터별로 만들 미니맵 머티리얼 인스턴스 변수.

public:
	UFUNCTION()
	void Set_HP_Text(float CurrentHP, float MaxHP);

	virtual void NativeOnInitialized() override;  // 위젯이 생성될때 한번 실행되는 함수

	//virtual void NativeConstruct() override;  // AddToViewport 될 때마다 실행되는 함수

	//virtual void NativeDestruct() override   // RemoveFromViewport 될 때마다  실행되는 함수

	void SetMinimapImage(class UTextureRenderTarget2D* PlayerMinimapRenderTarget);  // 미니맵 이미지에 머티리얼을 넣어주기 위한 함수 선언

	UFUNCTION()
	void SetFirstSkillProgress(float RemainingTime, float Cooldown);

	UFUNCTION()
	void SetSecondSkillProgress(float RemainingTime, float Cooldown);

	UFUNCTION()
	void WatchEmotionScreen();

	FORCEINLINE bool GetIsEmotionOverlayOpen() const{ return IsEmotionOverlayOpen; }
};
