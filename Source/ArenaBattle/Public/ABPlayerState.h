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
	// 경험치 저장 및 레벨변화 532p
	float GetExpRatio() const;
	bool AddExp(int32 IncomeExp);
	
	//델리게이트 생성 14장 525p
	
	FOnPlayerChangedDelegate OnPlayerStateChanged;

protected:
	// Transient 는 값이 수시로 변경되는 변수에 설정하는 것으로, 변수가 일시적으로 존재하며, 저장하지 않고 직렬화하지 않는 것을 나타낸다.
	// 게임 저장이나 로딩 등을 수행하지 않으며 게임 실행 중에만 유지됩니다. 
	// 이를 통해, 변수의 상태를 일시적으로 유지하면서도, 게임 데이터의 크기를 줄이고, 게임 실행 속도를 높일 수 있다.
	UPROPERTY(Transient)
	int32 GameScore;

	UPROPERTY(Transient)
	int32 CharacterLevel;

	// 경험치 저장 및 레벨변화 532p
	UPROPERTY(Transient)
	int32 Exp;

private:
	// 경험치 저장 및 레벨변화 532p
	void SetCharacterLevel(int32 NewCharacterLevel);
	// FABCharacterData 는 ABGameInstance에서 사용
	struct FABCharacterData* CurrentStatData;
};
