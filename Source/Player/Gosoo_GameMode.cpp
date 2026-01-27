// Fill out your copyright notice in the Description page of Project Settings.

// Project_Gosoo에서 사용할 게임임모드 클래스의 cpp파일.

#include "Gosoo_GameMode.h"   
#include "Gosoo_GameState.h"      // Gosoo_GameState클래스 헤더파일
#include "Gosoo_PlayerController.h"    // Gosoo_PlayerController 헤더파일
#include "Gosoo_PlayerState.h"   // Gosoo_PlayerState 헤더파일
#include "GosooPlayer.h"		// GosooPlayer 헤더파일
#include "UObject/ConstructorHelpers.h"    // 언리얼엔진 에디터내의 블루프린트를 cpp파일에서 참조하기 위한 헤더파일.

AGosoo_GameMode::AGosoo_GameMode()
{
	GameStateClass = AGosoo_GameState::StaticClass();    // 게임모드의 게임스테이트 클래스 설정을 Gosoo_GameState 클래스로 설정.
	PlayerStateClass = AGosoo_PlayerState::StaticClass();  // 게임모드의 플레이어스테이트 클래스 설정을 Gosoo_PlayerState 클래스로 설정.
	PlayerControllerClass = AGosoo_PlayerController::StaticClass(); // 게임모드의 플레이어 컨트롤러 클래스 설정을 Gosoo_PlayerController 클래스로 설정.

	static ConstructorHelpers::FClassFinder<APawn>PlayerPawnBPClass(TEXT("/Game/0_DEV/KYS/BP_GosooPlayer.BP_GosooPlayer_C"));   // BP_GosooPlayer를 가져온다.
	if (PlayerPawnBPClass.Class != nullptr)  // PlayerPawnBPClass가 존재하면
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;    // 게임모드의 DefaultPawnClass는 PlayerPawnBPClass(== BP_GosooPlayer)로 설정.
	}

}