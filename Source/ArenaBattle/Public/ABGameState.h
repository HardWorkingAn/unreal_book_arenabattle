// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ArenaBattle.h"

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "ABGameState.generated.h"

/**
 * 
 */
UCLASS()
class ARENABATTLE_API AABGameState : public AGameStateBase
{
	GENERATED_BODY()
	
public:
	AABGameState();

	int32 GetTotalGameScore() const;
	void AddGameScore();

	// 614p 미션 완료 되면 UI 출력
	void SetGameCleared();
	bool IsGameCleared() const;

private:
	UPROPERTY(Transient)
	int32 TotalGameScore;

	// 614p 미션 완료 되면 UI 출력
	UPROPERTY()
	bool bGameCleared;

};
