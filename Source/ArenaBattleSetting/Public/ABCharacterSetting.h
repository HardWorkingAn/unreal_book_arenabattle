// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ABCharacterSetting.generated.h"

// ABCharacterSetting  ������ ����� ĳ���� ������ ����� ����.
// ������ �ڵ忡 ĳ���� ������ �ڵ�� ������ �� ������ ���� ������ ����Ǹ� �ڵ带 �ٽ� ����� ������ �ؾ� �ϱ� ������
// �𸮾� ������Ʈ�� �⺻���� �����ϰ� ���� �ϵ��� �ܺ� INI���� ���� �⺻ �Ӽ����� �����ϴ� ��� ���
/**
 * 
 */
// �𸮾� �⺻���� INI���� �ҷ����Ϸ��� UCLASS�� config Ű���带 �߰��� ���⿡ �ҷ����� INI���� �̸��� �����ϰ� 
// �ҷ����� PROPERTY �Ӽ��� config Ű���带 �����ؾ��Ѵ�.
UCLASS(config=ArenaBattle) // config ���� Arenabattle�� �����ؼ� config ���� �ȿ� �ִ� DefaultArenaBattle.ini ������ �о�´�.
class ARENABATTLESETTING_API UABCharacterSetting : public UObject
{
	GENERATED_BODY()
	
public:
	UABCharacterSetting();

	UPROPERTY(config)
	//���� ��� ��ġ�� ���� �����ϴ� FSoftObjectPath
	TArray<FSoftObjectPath> CharacterAssets;

};
