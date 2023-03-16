// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ArenaBattle.h"
//#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ABGameMode.generated.h"

/**
 * 
 */
UCLASS()
class ARENABATTLE_API AABGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	AABGameMode();
	// 547p
	virtual void PostInitializeComponents() override;
	
	// 언리얼 엔진에서 플레이어가 게임에 입장하는것을 로그인이라 하며 로그인 과정에서 플레이어에게 할당된 플레이어 컨트룰러가 생성
	virtual void PostLogin(APlayerController* NewPlayer) override;

	// 547p
	void AddScore(class AABPlayerController *ScoredPlayer);

	// 15장 스코어 기반 AI레벨 설정
	int32 GetScore() const;

private:
	UPROPERTY()
	class AABGameState* ABGameState;
};
