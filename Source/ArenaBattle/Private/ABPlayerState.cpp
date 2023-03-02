// Fill out your copyright notice in the Description page of Project Settings.
#include "ABPlayerState.h"

// ����ġ ���� �� ������ȭ 532p
#include "ABGameInstance.h"

// 15�� 554p SaveGame
#include "ABSaveGame.h"

AABPlayerState::AABPlayerState()
{
	CharacterLevel = 1;
	GameScore = 0;
	Exp = 0;

	// 15�� GameSave���� 554p
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
	// 555p ���� �ٽ� �ۼ�
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

}

float AABPlayerState::GetExpRatio() const
{
	// KINDA_SMALL_NUMBER �� float ��Ʈ ���� �����ִ� ��Ʈ ������ 0���� ũ�� ��Ÿ����. �̸� �ذ��ϰ��� ���� �̼��� ������ �ذ��ϱ� ���� ���� ��
	// KINDA_SMALL_NUMBER �� 0�� �����, float���� 0�� ���Ҷ� 0���� KINDA_SMALL_NUMBER�� �����
	if (CurrentStatData->NextExp <= KINDA_SMALL_NUMBER)
	{
		return 0.0f;
	}
	float Result = (float)Exp / (float)CurrentStatData->NextExp; // ���� ����ġ / �ش� ������ �ϱ����� ����ġ ����
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
	return DidLevelUp;
}

void AABPlayerState::AddGameScore()
{
	GameScore++;
	
	// 15�� 556p �ڵ��߰�
	if(GameScore >= GameHighScore)
	{
		GameHighScore = GameScore;
	}
	//
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
