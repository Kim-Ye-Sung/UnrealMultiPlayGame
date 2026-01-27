// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "OptionSelectUI.generated.h"

/**
 * 
 */
UCLASS()
class GOSOO_API UOptionSelectUI : public UUserWidget
{
	GENERATED_BODY()
	
	UPROPERTY(meta =(BindWidget))
	class UButton* SoundOptionButton;

	UPROPERTY(meta = (BindWidget))
	UButton* DisplayOptionButton;

	UPROPERTY(meta = (BindWidget))
	UButton* KeySettingOptionButton;

	UPROPERTY(meta = (BindWidget))
	UButton* MouseSettingOptionButton;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* SoundText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* DisplayText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* KeySettingText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* MouseSettingText;

	virtual void NativeOnInitialized() override;

	virtual void NativeConstruct() override;

	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;
	
	UFUNCTION()
	void SoundOptionWidgetOpen();          // 사운드 설정창UI를 켜는 함수

	UFUNCTION()
	void DisplayOptionWidgetOpen();

	UFUNCTION()
	void KeySettingOptionWidgetOpen();

	UFUNCTION()
	void MouseSettingOptionWidgetOpen();

	UFUNCTION()
	void SetCurrentOptionWidget(UUserWidget* CurrentOptionObj);

	UPROPERTY()
	TObjectPtr<UTextBlock> CurrentOptionText;

	UPROPERTY()
	TObjectPtr<UTextBlock> HoveredText;

	UFUNCTION()
	void SetCurrentOptionText(UTextBlock* OptionText);

	UPROPERTY()
	bool IsMouseHovered = false;  // 현재 마우스가 호버드 상태인지 아닌지 판단하는 변수. 처음은 아무것도 아니니 unhovered인 false로 시작.

	UFUNCTION()
	void SetSoundTextColor();       // 사운드 텍스트의 색깔을 변경하는 함수 

	UFUNCTION()
	void SetDisplayTextColor();

	UFUNCTION()
	void SetKeySettingTextColor();

	UFUNCTION()
	void SetMouseSettingTextColor();

	UFUNCTION()
	void HoveredTextColorChange();

	UPROPERTY(EditAnywhere, Category = "ReferencesRequired|UI")
	TSubclassOf<UUserWidget> SoundOptionWidgetClass;

	UPROPERTY(EditAnywhere, Category = "ReferencesRequired|UI")
	TSubclassOf<UUserWidget> DisplayOptionWidgetClass;

	UPROPERTY(EditAnywhere, Category = "ReferencesRequired|UI")
	TSubclassOf<UUserWidget> KeySettingOptionWidgetClass;

	UPROPERTY(EditAnywhere, Category = "ReferencesRequired|UI")
	TSubclassOf<UUserWidget> MouseSettingOptionWidgetClass;

	UPROPERTY()
	TObjectPtr<UUserWidget> SoundOptionWidgetObj;

	UPROPERTY()
	TObjectPtr<UUserWidget> DisplayOptionWidgetObj;

	UPROPERTY()
	TObjectPtr<UUserWidget> KeySettingOptionWidgetObj;
	
	UPROPERTY()
	TObjectPtr<UUserWidget> MouseSettingOptionWidgetObj;

	UPROPERTY()
	TObjectPtr<UUserWidget> CurrentOptionWidget;

	UPROPERTY()
	TObjectPtr<class AGosooPlayer> OwnerPlayer;     // 플레이어가 마우스 클릭으로 공격기능 실행하지 않고 UI만 클릭할수 있도록 캐릭터를 참조.

	UFUNCTION()
	void OpenOptionInputMode();

	UFUNCTION()
	void CloseOptionInputMode();

public:
	FORCEINLINE void SetOwnerPlayer(AGosooPlayer* Player) { OwnerPlayer = Player; }
};
