// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ArenaBattle.h"

#include "Engine/DataTable.h" // FTableRowBase 를 가지고 있는 해더파일
#include "CoreMinimal.h"
#include "Engine/GameInstance.h"

// 13장 INI 파일 가져온거 npc 랜덤으로  467p 
#include "Engine/StreamableManager.h"

#include "ABGameInstance.generated.h"



// GameInstance는 게임이 시작할 때 만들어져서 게임이 종료될 때 까지 프로그램 상에 존재
// 프로그램이 종료될때 까지 살아있는 게임 인스턴스 안에 중요한 데이터들을 저장하고, 또 그것을 꺼내서 사용하는 로직을 주로 사용
// csv를 읽기 위한 FTableRowBase 를 상속받는 구조체 생성
USTRUCT(BlueprintType)
struct FABCharacterData : public FTableRowBase
{
	GENERATED_BODY()

public:
	FABCharacterData() : Level(1), MaxHP(100.0f), Attack(10.0f), DropExp(10), NextExp(30) {} //초기값 설정
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int32 Level;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	float MaxHP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	float Attack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int32 DropExp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int32 NextExp;
};


/**
 * 
 */
UCLASS()
class ARENABATTLE_API UABGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	UABGameInstance();

	virtual void Init() override;

	FABCharacterData* GetABCharacterData(int32 Level);

	// 468p
	FStreamableManager StreamableManager;

private:
	UPROPERTY()
	class UDataTable* ABCharacterTable;
		                 
};
