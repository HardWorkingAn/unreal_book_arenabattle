// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ArenaBattle.h"

#include "Engine/DataTable.h" // FTableRowBase �� ������ �ִ� �ش�����
#include "CoreMinimal.h"
#include "Engine/GameInstance.h"

// 13�� INI ���� �����°� npc ��������  467p 
#include "Engine/StreamableManager.h"

#include "ABGameInstance.generated.h"



// GameInstance�� ������ ������ �� ��������� ������ ����� �� ���� ���α׷� �� ����
// ���α׷��� ����ɶ� ���� ����ִ� ���� �ν��Ͻ� �ȿ� �߿��� �����͵��� �����ϰ�, �� �װ��� ������ ����ϴ� ������ �ַ� ���
// csv�� �б� ���� FTableRowBase �� ��ӹ޴� ����ü ����
USTRUCT(BlueprintType)
struct FABCharacterData : public FTableRowBase
{
	GENERATED_BODY()

public:
	FABCharacterData() : Level(1), MaxHP(100.0f), Attack(10.0f), DropExp(10), NextExp(30) {} //�ʱⰪ ����
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
