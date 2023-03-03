// Fill out your copyright notice in the Description page of Project Settings.
#include "ABPlayerState.h"

// 경험치 저장 및 레벨변화 532p
#include "ABGameInstance.h"

// 15장 554p SaveGame
#include "ABSaveGame.h"

AABPlayerState::AABPlayerState()
{
	CharacterLevel = 1;
	GameScore = 0;
	Exp = 0;

	// 15장 GameSave파일 554p
	GameHighScore = 0;
	SaveSlotName = TEXT("Player1");
}

int32 AABPlayerState::GetGameScore() const
{
	return GameScore;
}

int32 AABPlayerState::GetCharacterLevel() const
{
	return CharacterLevel;
}

int32 AABPlayerState::GetGameHighScroe() const
{
	return GameHighScore;
}

void AABPlayerState::InitPlayerData()
{
	// 555p 에서 다시 작성
	/*
	SetPlayerName(TEXT("Destiny"));
	//CharacterLevel = 5;
	SetCharacterLevel(5);
	Exp = 0;
	GameScore = 0;
	*/

	auto ABSaveGmae = Cast<UABSaveGame>(UGameplayStatics::LoadGameFromSlot(SaveSlotName, 0));
	if (nullptr == ABSaveGmae)
	{
		ABSaveGmae = GetMutableDefault<UABSaveGame>();
	}
	SetPlayerName(ABSaveGmae->PlayerName);
	SetCharacterLevel(ABSaveGmae->Level);
	GameScore = 0;
	GameHighScore = ABSaveGmae->HighScore;
	Exp = ABSaveGmae->Exp;
	//경험치 변동이 있을때 마다 저장 557p 추가
	SavePlayerData();
}

void AABPlayerState::SavePlayerData()
{
	UABSaveGame* NewPlayerData = NewObject<UABSaveGame>();
	NewPlayerData->PlayerName = GetPlayerName();
	NewPlayerData->Level = CharacterLevel;
	NewPlayerData->Exp = Exp;
	NewPlayerData->HighScore = GameHighScore;

	if (!UGameplayStatics::SaveGameToSlot(NewPlayerData, SaveSlotName, 0))
	{
		ABLOG(Error, TEXT("Save Game Error!"));
	}

}

float AABPlayerState::GetExpRatio() const
{
	// KINDA_SMALL_NUMBER 는 float 비트 끝에 남아있는 비트 떄문에 0보다 크게 나타난다. 이를 해결하고자 좀더 미세한 오차를 해결하기 위해 쓰는 값
	// KINDA_SMALL_NUMBER 은 0에 가까움, float값을 0과 비교할때 0보다 KINDA_SMALL_NUMBER을 사용함
	if (CurrentStatData->NextExp <= KINDA_SMALL_NUMBER)
	{
		return 0.0f;
	}
	float Result = (float)Exp / (float)CurrentStatData->NextExp; // 현재 경험치 / 해당 레벨업 하기위한 경험치 비율
	ABLOG(Warning, TEXT("Ratio : %f, Current : %d, Next : %d"), Result, Exp, CurrentStatData->NextExp);
	return Result;
}

bool AABPlayerState::AddExp(int32 IncomeExp)
{
	if (CurrentStatData->NextExp == -1)
	{
		return false;
	}

	bool DidLevelUp = false;
	Exp = Exp + IncomeExp;
	if (Exp >= CurrentStatData->NextExp)
	{
		Exp -= CurrentStatData->NextExp;
		SetCharacterLevel(CharacterLevel + 1);
		DidLevelUp = true;
	}
	OnPlayerStateChanged.Broadcast();
	//경험치 변동이 있을때 마다 저장 557p 추가
	SavePlayerData();
	return DidLevelUp;
}

void AABPlayerState::AddGameScore()
{
	GameScore++;
	
	// 15장 556p 코드추가
	if(GameScore >= GameHighScore)
	{
		GameHighScore = GameScore;
	}
	//경험치 변동이 있을때 마다 저장 557p 추가
	SavePlayerData();
	OnPlayerStateChanged.Broadcast();
}

void AABPlayerState::SetCharacterLevel(int32 NewCharacterLevel)
{
	auto ABGameInstance = Cast<UABGameInstance>(GetGameInstance());
	ABCHECK(nullptr != ABGameInstance);

	CurrentStatData = ABGameInstance->GetABCharacterData(NewCharacterLevel);
	ABCHECK(nullptr != CurrentStatData);

	CharacterLevel = NewCharacterLevel;
}
