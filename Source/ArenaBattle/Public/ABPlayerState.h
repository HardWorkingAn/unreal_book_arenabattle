// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "ABPlayerState.generated.h"

/**
 * 
 */

DECLARE_MULTICAST_DELEGATE(FOnPlayerChangedDelegate);

UCLASS()
class ARENABATTLE_API AABPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	AABPlayerState();

	int32 GetGameScore() const;
	int32 GetCharacterLevel() const;
	
	void InitPlayerData();
	// ����ġ ���� �� ������ȭ 532p
	float GetExpRatio() const;
	bool AddExp(int32 IncomeExp);
	
	//��������Ʈ ���� 14�� 525p
	
	FOnPlayerChangedDelegate OnPlayerStateChanged;

protected:
	// Transient �� ���� ���÷� ����Ǵ� ������ �����ϴ� ������, ������ �Ͻ������� �����ϸ�, �������� �ʰ� ����ȭ���� �ʴ� ���� ��Ÿ����.
	// ���� �����̳� �ε� ���� �������� ������ ���� ���� �߿��� �����˴ϴ�. 
	// �̸� ����, ������ ���¸� �Ͻ������� �����ϸ鼭��, ���� �������� ũ�⸦ ���̰�, ���� ���� �ӵ��� ���� �� �ִ�.
	UPROPERTY(Transient)
	int32 GameScore;

	UPROPERTY(Transient)
	int32 CharacterLevel;

	// ����ġ ���� �� ������ȭ 532p
	UPROPERTY(Transient)
	int32 Exp;

private:
	// ����ġ ���� �� ������ȭ 532p
	void SetCharacterLevel(int32 NewCharacterLevel);
	// FABCharacterData �� ABGameInstance���� ���
	struct FABCharacterData* CurrentStatData;
};
