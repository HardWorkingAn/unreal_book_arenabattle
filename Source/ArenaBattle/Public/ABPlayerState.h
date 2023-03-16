// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ArenaBattle.h"

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "ABPlayerState.generated.h"

/**
 * 
 */

// 현제 게임에서 플레이어 레벨, 체력, 경험치, 점수를 저장하는 곳
// 외부 클래스에서 ABPlayerState 함수를 이용해 레벨 체력 경험치 점수를 설정함
// 플레이 하고있는 현제 캐릭터의 State를 나타내는 곳

DECLARE_MULTICAST_DELEGATE(FOnPlayerChangedDelegate);

UCLASS()
class ARENABATTLE_API AABPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	AABPlayerState();

	int32 GetGameScore() const;
	int32 GetCharacterLevel() const;
	
	// 15장 GameSave파일 554p
	int32 GetGameHighScroe() const;
	FString SaveSlotName;
	// 595p 선택한 캐릭터 Index 가져오기
	int32 GetCharacterIndex() const;
	//

	void InitPlayerData();
	// 경험치 변동이 있을때 마다 저장 557p 추가
	void SavePlayerData();
	// 경험치 저장 및 레벨변화 532p
	float GetExpRatio() const;
	bool AddExp(int32 IncomeExp);
	
	// 546p 추가
	void AddGameScore();
	//델리게이트 생성 14장 525p
	
	FOnPlayerChangedDelegate OnPlayerStateChanged;

protected:
	// Transient 는 값이 수시로 변경되는 변수에 설정하는 것으로, 변수가 일시적으로 존재하며, 저장하지 않고 직렬화하지 않는 것을 나타낸다.
	// 게임 저장이나 로딩 등을 수행하지 않으며 게임 실행 중에만 유지됩니다. 
	// 이를 통해, 변수의 상태를 일시적으로 유지하면서도, 게임 데이터의 크기를 줄이고, 게임 실행 속도를 높일 수 있다.
	UPROPERTY(Transient)
	int32 GameScore;

	// 15장 GameSave파일 554p
	UPROPERTY(Transient)
	int32 GameHighScore;
	//

	UPROPERTY(Transient)
	int32 CharacterLevel;

	// 경험치 저장 및 레벨변화 532p
	UPROPERTY(Transient)
	int32 Exp;

	// 595p 선택한 캐릭터 Index 가져오기
	UPROPERTY(Transient)
	int32 CharacterIndex;
private:
	// 경험치 저장 및 레벨변화 532p
	void SetCharacterLevel(int32 NewCharacterLevel);
	// FABCharacterData 는 ABGameInstance에서 사용
	struct FABCharacterData* CurrentStatData;
};
