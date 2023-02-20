// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ArenaBattle.h"
//#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ABGameMode.generated.h"

/**
 * 
 */
UCLASS()
class ARENABATTLE_API AABGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	AABGameMode();

	// �𸮾� �������� �÷��̾ ���ӿ� �����ϴ°��� �α����̶� �ϸ� �α��� �������� �÷��̾�� �Ҵ�� �÷��̾� ��Ʈ�귯�� ����
	virtual void PostLogin(APlayerController* NewPlayer) override;
};
