// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AreanBattle.h"

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ABHUDWidget.generated.h"

/**
 * 
 */
UCLASS()
class ARENABATTLE_API UABHUDWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	void BindCharacterStat(class UABCharacterStatCompoenent* CharacterStat);
	void BindPlayerState(class AABPlayerState* PlayerState);

protected:
	virtual void NativeConstruct() override;
	void UpdateCharacterStat();
	void UpdatePlayerState();
};
