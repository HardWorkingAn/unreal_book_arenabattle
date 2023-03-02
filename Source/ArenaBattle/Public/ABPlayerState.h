// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ArenaBattle.h"

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "ABPlayerState.generated.h"

/**
 * 
 */

// ���� ���ӿ��� �÷��̾� ����, ü��, ����ġ, ������ �����ϴ� ��
// �ܺ� Ŭ�������� ABPlayerState �Լ��� �̿��� ���� ü�� ����ġ ������ ������

DECLARE_MULTICAST_DELEGATE(FOnPlayerChangedDelegate);

UCLASS()
class ARENABATTLE_API AABPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	AABPlayerState();

	int32 GetGameScore() const;
	int32 GetCharacterLevel() const;
	
	// 15�� GameSave���� 554p
	int32 GetGameHighScroe() const;
	FString SaveSlotName;
	//

	void InitPlayerData();
	// ����ġ ���� �� ������ȭ 532p
	float GetExpRatio() const;
	bool AddExp(int32 IncomeExp);
	
	// 546p �߰�
	void AddGameScore();
	//��������Ʈ ���� 14�� 525p
	
	FOnPlayerChangedDelegate OnPlayerStateChanged;

protected:
	// Transient �� ���� ���÷� ����Ǵ� ������ �����ϴ� ������, ������ �Ͻ������� �����ϸ�, �������� �ʰ� ����ȭ���� �ʴ� ���� ��Ÿ����.
	// ���� �����̳� �ε� ���� �������� ������ ���� ���� �߿��� �����˴ϴ�. 
	// �̸� ����, ������ ���¸� �Ͻ������� �����ϸ鼭��, ���� �������� ũ�⸦ ���̰�, ���� ���� �ӵ��� ���� �� �ִ�.
	UPROPERTY(Transient)
	int32 GameScore;

	// 15�� GameSave���� 554p
	UPROPERTY(Transient)
	int32 GameHighScore;
	//

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
