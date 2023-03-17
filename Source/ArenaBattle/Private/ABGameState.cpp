// Fill out your copyright notice in the Description page of Project Settings.

#include "ABGameState.h"

AABGameState::AABGameState()
{
	TotalGameScore = 0;

	// 614p 추가
	bGameCleared = false;
}

int32 AABGameState::GetTotalGameScore() const
{
	return TotalGameScore;
}

void AABGameState::AddGameScore()
{
	TotalGameScore++;
}

// 614p 기본작성
void AABGameState::SetGameCleared()
{
	bGameCleared = true;
}

// 614p 기본작성
bool AABGameState::IsGameCleared() const
{
	return bGameCleared;
}
