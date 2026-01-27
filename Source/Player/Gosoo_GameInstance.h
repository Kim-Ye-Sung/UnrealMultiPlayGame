// Fill out your copyright notice in the Description page of Project Settings.

// Project_Gosoo에서 사용할 게임인스턴스 클래스의 헤더파일.
// 게임인스턴스는 레벨 변경시 클라이언트별로 저장된 정보를 그대로 들고갈때 사용한다.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Gosoo_GameInstance.generated.h"

/**
 * 
 */
UCLASS()
class GOSOO_API UGosoo_GameInstance : public UGameInstance
{
	GENERATED_BODY()
	
};
