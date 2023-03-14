// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ArenaBattle.h"

#include "CoreMinimal.h"
#include "AIController.h"
#include "ABAIController.generated.h"

/**
 * 
 */
UCLASS()
class ARENABATTLE_API AABAIController : public AAIController
{
	GENERATED_BODY()

// 폰에게 3초마다 목적지로 이동하는 명령
// 언리얼 네비게이션은 이동가능한 목적지를 랜덤으로 가져오는 GetRandomPointInNavigableRTadius 함수와 목적지로 폰을 이동시키는 SimpleMoveToLocation 함수 제공
public:
	AABAIController();
	// AI 랜덤위치 이동
	virtual void OnPossess(APawn* InPawn) override;
	//virtual void OnUnPossess() override;

	//블랙보드 에서 키 추가 한거 연결
	static const FName HomePosKey;
	static const FName PatrolPosKey;

	static const FName TargetKey;

	// 14장 509p
	void RunAI();
	void StopAI();
private:
	// AI 랜덤위치 이동
	/*
	void OnRepeatTimer();
	FTimerHandle RePeatTimerHandle;
	float ReapeatInterval;
	*/
	// 비헤이비어 트리 및 블랙보드 사용 AI이동
	UPROPERTY()
		class UBehaviorTree* BTAsset;

	UPROPERTY()
		class UBlackboardData* BBAsset;
};
