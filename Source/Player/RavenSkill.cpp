// Fill out your copyright notice in the Description page of Project Settings.


#include "RavenSkill.h"
#include "GosooPlayer.h"
#include "NavigationSystem.h"
#include "NavigationPath.h"
#include "NavMesh/RecastNavMesh.h"
#include "GameFramework/CharacterMovementComponent.h"

URavenSkill::URavenSkill()
{
	ComboSkill = true;   // 1번스킬을 콤보스킬로 설정. 따라서 한번 누른것으로는 1번키를 못쓰게 할 순 없음.
}

void URavenSkill::Skill_1_Excute()
{
	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Cyan, TEXT("HAHA"));

	switch (Combo)
	{
	case 0:
	{
		if (AGosooPlayer* OwnerPlayer = Cast<AGosooPlayer>(GetOwner()))
		{
			if (OwnerPlayer->PlayerAnimInstance)
			{
				OwnerPlayer->PlayerAnimInstance->Montage_Play(SkillMontage, 1.0f);

				Combo++;
			}
		}
		break;
	}
	case 1:
	{
		if (AGosooPlayer* OwnerPlayer = Cast<AGosooPlayer>(GetOwner()))
		{
			if (OwnerPlayer->PlayerAnimInstance)
			{
				//OwnerPlayer->PlayerAnimInstance->Montage_JumpToSection(TEXT("SecondPose"), SkillMontage);
				OwnerPlayer->PlayerAnimInstance->Montage_Play(SkillMontage2, 1.0f);

				Combo++;
			}
		}
		CoolDown_On = true;
		break;
	}
	case 2:
	{
		if (HasGoalTransform)
		{
            GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, TEXT("Skill_Combo_Excute"));

			CanSkill1 = false;  // 더이상 스킬 사용 불가
			Combo = 0;

			StartMoveToGoalLocation();
		}
		break;
	}
	}

	// 1번을 누르면 쿨타임은 공을 손에 들고 쿨타임이 돌지는 않는다.
	// 공을 던지는순간부터 쿨타임이 돈다.
	// 공이 굴러가는데 적과 충돌하면, 터진다.
	// 터질때 주변범위내의 적들한테 데미지를 준다.(이때는 마킹이 남지 않는다.)
	// 혹은 공이 굴러가는데, 적과 충돌하지 않고, 벽과도 충돌하지 않아으면 타이머가 되서 터진다.
	// 이때도 주변 범위내의 적들한테 데미지를 준다.(이때도 마킹이 남지 않는다.)
	// 그런데 굴러가는데, 벽과 충돌하면 터지는건 절대 없고, 벽에 마킹이 남는다.
	// 그리고 다시 한번 스킬을 누르면 이 위치로 이동한다.(순간이동이 끝나고 0.5초 짧게 무적시간)

}

void URavenSkill::Skill_2_Excute()
{
}




// ---------------------------------- 성공했음 --------------------------------------

void URavenSkill::StartMoveToGoalLocation()
{
    UNavigationSystemV1* NavSys = UNavigationSystemV1::GetCurrent(GetWorld());
    if (!NavSys)
    {
        //GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, TEXT("NavSys is NULL"));
        return;
    }

    // ANavigationData 가져오기
    ANavigationData* NavData = NavSys->GetDefaultNavDataInstance();
    if (!NavData)
    {
        //GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, TEXT("NavData is NULL"));
        return;
    }

    // SkillGoalLocation을 Nav Mesh 위로 투영
    FNavLocation ProjectedLocation;
    if (NavSys->ProjectPointToNavigation(SkillGoalLocation, ProjectedLocation, FVector(500.f, 500.f, 500.f)))
    {
        FVector SaveLocation = SkillGoalLocation;

        SkillGoalLocation = ProjectedLocation.Location;

        SkillGoalLocation.Z = SaveLocation.Z;
        //GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Yellow, TEXT("Goal Location Projected"));
    }
    else
    {
        //GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, TEXT("Cannot Project Goal Location"));
        return;
    }

    // 경로 계산 - 더 완전한 Query 설정
    FPathFindingQuery Query(
        OwnerCharacter,                          // Querier
        *NavData,                                 // NavData
        OwnerCharacter->GetActorLocation(),      // Start Location
        SkillGoalLocation                        // End Location
    );

    FPathFindingResult Result = NavSys->FindPathSync(Query);

    // 디버그 메시지 추가
    if (!Result.IsSuccessful())
    {
        GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, TEXT("Path Finding Failed!"));
        return;
    }

    if (!Result.Path.IsValid())
    {
        GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, TEXT("Path is Invalid!"));
        return;
    }

    GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, TEXT("333333"));

    PathPoints.Empty();
    for (const FNavPathPoint& Point : Result.Path->GetPathPoints())
    {
        PathPoints.Add(Point.Location);
    }

    if (PathPoints.Num() > 0)
    {
        GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Cyan,
            FString::Printf(TEXT("Path Points: %d"), PathPoints.Num()));

        CurrentPathIndex = 0;

        // 전체 경로 길이 계산
        float TotalDistance = 0.f;
        for (int32 i = 0; i < PathPoints.Num() - 1; i++)
        {
            TotalDistance += FVector::Dist(PathPoints[i], PathPoints[i + 1]);
        }

        // 3초 동안 이동하려면 필요한 속도 계산
        MoveSpeed = TotalDistance / 3.0f;

        GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Magenta,
            FString::Printf(TEXT("MoveSpeed: %f, TotalDistance: %f"), MoveSpeed, TotalDistance));

        // 경로 그리기 (디버그용)
        const TArray<FNavPathPoint>& Points = Result.Path->GetPathPoints();
        for (int32 i = 0; i < Points.Num() - 1; i++)
        {
            DrawDebugLine(GetWorld(), Points[i].Location, Points[i + 1].Location,
                FColor::Green, false, 5.0f, 0, 5.0f);
        }

        // CharacterMovementComponent 비활성화 (있다면)
        UCharacterMovementComponent* MovementComp = OwnerCharacter->FindComponentByClass<UCharacterMovementComponent>();
        if (MovementComp)
        {
            MovementComp->DisableMovement();
            GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Orange, TEXT("Movement Component Disabled"));
        }

        // 타이머로 이동 시작
        GetWorld()->GetTimerManager().SetTimer(MoveTimerHandle, this,
            &URavenSkill::FollowPath, 0.016f, true);
    }
}

void URavenSkill::FollowPath()
{
    if (!OwnerCharacter)
    {
        GetWorld()->GetTimerManager().ClearTimer(MoveTimerHandle);
        return;
    }

    if (CurrentPathIndex >= PathPoints.Num())
    {
        GetWorld()->GetTimerManager().ClearTimer(MoveTimerHandle);

        // CharacterMovementComponent 다시 활성화
        UCharacterMovementComponent* MovementComp = OwnerCharacter->FindComponentByClass<UCharacterMovementComponent>();
        if (MovementComp)
        {
            MovementComp->SetMovementMode(MOVE_Walking);
        }

        GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, TEXT("Movement Complete!"));
        return;
    }

    FVector CurrentLocation = OwnerCharacter->GetActorLocation();
    FVector TargetPoint = PathPoints[CurrentPathIndex];

    const float HalfHeight = OwnerCharacter->GetSimpleCollisionHalfHeight();

    TargetPoint.Z += HalfHeight;

    // 디버그: 위치 변화 확인
    static FVector LastLocation = CurrentLocation;
    float MovedDistance = FVector::Dist(LastLocation, CurrentLocation);

    GEngine->AddOnScreenDebugMessage(-1, 0.016f, FColor::Yellow,
        FString::Printf(TEXT("CurrentIndex: %d/%d, MovedDist: %.2f, CurrentLoc: %s"),
            CurrentPathIndex, PathPoints.Num() - 1, MovedDistance, *CurrentLocation.ToString()));

    LastLocation = CurrentLocation;

    FVector Direction = (TargetPoint - CurrentLocation).GetSafeNormal();
    float DistanceToTarget = FVector::Dist(CurrentLocation, TargetPoint);
    float MoveDistance = MoveSpeed * 0.016f; // DeltaTime

    if (DistanceToTarget <= 50.0f)  // 50cm 이내면 다음 포인트로
    {
        CurrentPathIndex++;
        GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Cyan,
            FString::Printf(TEXT("Moving to next point: %d"), CurrentPathIndex));
    }
    else
    {
        FVector NewLocation = CurrentLocation + Direction * MoveDistance;

        // 위치 설정 전후 비교
        FVector BeforeLocation = OwnerCharacter->GetActorLocation();
        bool bSuccess = OwnerCharacter->SetActorLocation(NewLocation, false);
        FVector AfterLocation = OwnerCharacter->GetActorLocation();

        if (!bSuccess || BeforeLocation.Equals(AfterLocation, 1.0f))
        {
            GEngine->AddOnScreenDebugMessage(-1, 0.016f, FColor::Red,
                TEXT("SetActorLocation FAILED or NO CHANGE!"));
        }
    }
}

//------------------------------------------------------------------------------------------------------

//void URavenSkill::MoveToLocationWithNavMesh()
//{
//
//	// 1. 현재 월드의 Navigation System을 가져옵니다
//	// Navigation System은 NavMesh를 관리하고 경로 찾기를 담당하는 시스템입니다
//	UNavigationSystemV1* NavSys = UNavigationSystemV1::GetCurrent(GetWorld());
//
//    // 2. Navigation System이 유효한지 확인 (NavMesh가 없으면 nullptr)
//    if (NavSys)
//    {
//
//        // 3. NavMesh 위의 위치 정보를 저장할 구조체
//        // FNavLocation은 NavMesh 위의 정확한 위치와 관련 정보를 담습니다
//        FNavLocation NavLocation;
//
//        // 4. 목표 위치(GoalLocation)를 NavMesh 위의 가장 가까운 유효한 위치로 투영
//        // 예: GoalLocation이 공중이나 벽 안쪽이면 가장 가까운 NavMesh 표면으로 조정
//        if (NavSys->ProjectPointToNavigation(SkillGoalLocation, NavLocation))
//        {
//
//            GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, TEXT("333333"));
//            // 5. 경로 찾기 쿼리(질문) 구조체를 생성
//            // 이 구조체에 "어디서 어디로 가고 싶은지" 정보를 담습니다
//            FPathFindingQuery Query;
//
//            // 6. 출발 위치 설정 - 캐릭터의 현재 위치
//            Query.StartLocation = OwnerCharacter->GetActorLocation();
//
//            // 7. 도착 위치 설정 - NavMesh 위로 투영된 목표 위치
//            Query.EndLocation = NavLocation.Location;
//
//            // 8. 어떤 NavMesh 데이터를 사용할지 지정 (기본 NavMesh 사용)
//            // 게임에 여러 종류의 NavMesh가 있을 수 있어서 지정이 필요합니다
//            Query.NavData = NavSys->GetDefaultNavDataInstance();
//
//            // 9. 실제로 경로를 찾습니다 (동기 방식 - 결과가 나올 때까지 대기)
//            // FindPathSync = Find Path Synchronously (동기적으로 경로 찾기)
//            FPathFindingResult Result = NavSys->FindPathSync(Query);
//
//            // 10. 경로 찾기가 성공했는지 && 경로 데이터가 유효한지 확인
//            if (Result.IsSuccessful() && Result.Path.IsValid())
//            {
//                GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, TEXT("4444444"));
//
//                // 11. 찾은 경로의 포인트들을 배열로 저장
//                // 경로는 여러 개의 점(포인트)들로 이루어져 있습니다
//                // 예: 시작점 → 모퉁이1 → 모퉁이2 → 목표점
//                PathPoints = Result.Path->GetPathPoints();
//
//                // 12. 현재 인덱스를 0으로 초기화 (첫 번째 포인트부터 시작)
//                CurrentPathIndex = 0;
//
//                // 13. 전체 경로의 길이를 저장 (3초 이동 속도 계산에 사용)
//                TotalPathLength = Result.Path->GetLength();
//
//                // 14. 타이머를 설정하여 매 프레임마다 FollowPathTick 함수 호출
//                // 0.016f = 약 1/60초 = 60FPS로 업데이트
//                // true = 반복 실행 (한 번만 실행하려면 false)
//                GetWorld()->GetTimerManager().SetTimer(
//                    MoveTimerHandle,           // 타이머 핸들 (나중에 중지할 때 사용)
//                    this,                      // 호출할 객체 (이 캐릭터)
//                    &URavenSkill::FollowPathTick,			   // 호출할 함수
//                    0.016f,                    // 호출 간격 (초 단위)
//                    true                       // 반복 여부
//                );
//            }
//        }
//    }
//}
//
//void URavenSkill::FollowPathTick()
//{    // 15. 모든 경로 포인트를 다 방문했는지 확인
//    if (CurrentPathIndex >= PathPoints.Num())
//    {
//        // 16. 목적지 도착 - 타이머를 중지하여 더 이상 이동하지 않음
//        GetWorld()->GetTimerManager().ClearTimer(MoveTimerHandle);
//        return; // 함수 종료
//    }
//
//    // 17. 현재 목표로 하는 경로 포인트의 위치를 가져옴
//    FVector CurrentTarget = PathPoints[CurrentPathIndex].Location;
//
//    // 18. 캐릭터의 현재 위치를 가져옴
//    FVector CurrentLocation = OwnerCharacter->GetActorLocation();
//
//    // 19. 현재 위치에서 목표 포인트로 가는 방향 벡터 계산
//    // GetSafeNormal() = 정규화된 방향 벡터 (길이 1로 만듦, 0벡터면 안전하게 처리)
//    FVector Direction = (CurrentTarget - CurrentLocation).GetSafeNormal();
//
//    // 20. 이동 속도 계산 (전체 경로를 3초에 걸쳐 이동하도록)
//    // 전체 경로 길이 / 3초 = 초당 이동해야 할 거리
//    float MoveSpeed = (TotalPathLength / 3.0f);
//
//    // 21. 이번 프레임에 이동할 새 위치 계산
//    // 현재 위치 + (방향 * 속도 * 델타타임)
//    // 0.016f는 약 1/60초이므로 매우 작은 거리씩 이동
//    FVector NewLocation = CurrentLocation + Direction * MoveSpeed * 0.016f;
//
//    // 22. 캐릭터를 새 위치로 이동
//    // true = Sweep 활성화 (이동 경로에 장애물이 있으면 충돌 감지하여 막힘)
//    // Sweep을 켜면 벽을 뚫고 지나가지 않습니다
//    OwnerCharacter->SetActorLocation(NewLocation, true);
//
//    // 23. 현재 목표 포인트에 충분히 가까워졌는지 확인
//    // 50.0f = 허용 오차 (50cm 이내면 도착한 것으로 간주)
//    if (FVector::Dist(CurrentLocation, CurrentTarget) < 50.0f)
//    {
//        // 24. 다음 경로 포인트로 인덱스 증가
//        // 이제 다음 포인트를 향해 이동하게 됩니다
//        CurrentPathIndex++;
//    }
//}


