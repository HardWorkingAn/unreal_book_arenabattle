// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ArenaBattle.h"

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "ABSaveGame.generated.h"

/**
 * 
 */

// 세이브 데이터를 만들어 저장하는 USaveGame 부모클래스

UCLASS()
class ARENABATTLE_API UABSaveGame : public USaveGame
{
	GENERATED_BODY()
	
public:
	UABSaveGame();

	UPROPERTY()
	int32 Level;

	UPROPERTY()
	int32 Exp;

	UPROPERTY()
	FString PlayerName;

	UPROPERTY()
	int32 HighScore;
};
