// Fill out your copyright notice in the Description page of Project Settings.

#include "ArenaBattle.h"
#include "ABPlayerState.h"

// ����ġ ���� �� ������ȭ 532p
#include "ABGameInstance.h"

AABPlayerState::AABPlayerState()
{
	CharacterLevel = 1;
	GameScore = 0;

	Exp = 0;
}

int32 AABPlayerState::GetGameScore() const
{
	return GameScore;
}

int32 AABPlayerState::GetCharacterLevel() const
{
	return CharacterLevel;
}

void AABPlayerState::InitPlayerData()
{
	SetPlayerName(TEXT("Destiny"));
	//CharacterLevel = 5;
	SetCharacterLevel(5);
	Exp = 0;
	GameScore = 0;
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

void AABPlayerState::SetCharacterLevel(int32 NewCharacterLevel)
{
	auto ABGameInstance = Cast<UABGameInstance>(GetGameInstance());
	ABCHECK(nullptr != ABGameInstance);

	CurrentStatData = ABGameInstance->GetABCharacterData(NewCharacterLevel);
	ABCHECK(nullptr != CurrentStatData);

	CharacterLevel = NewCharacterLevel;
}
