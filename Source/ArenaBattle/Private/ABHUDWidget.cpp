﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "ABHUDWidget.h"

#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "ABCharacterStatComponent.h"
#include "ABPlayerState.h"



void UABHUDWidget::BindCharacterStat(class UABCharacterStatComponent* CharacterStat)
{
	ABCHECK(nullptr != CharacterStat);
	CurrentCharacterStat = CharacterStat;
	// OnHPChanged는 델리게이트이며 델리게이트 안에 UpdateCharacterStat 함수를 포함시켜 OnHpChanged를 호출하면 포함시킨 UpdateCharacterStat 함수 작동
	CharacterStat->OnHPChanged.AddUObject(this, &UABHUDWidget::UpdateCharacterStat); 
}

void UABHUDWidget::BindPlayerState(AABPlayerState* PlayerState)
{
	ABCHECK(nullptr != PlayerState);
	CurrentPlayerState = PlayerState;
	// OnPlayerStateChanged는 델리게이트이며 델리게이트 안에 UpdatePlayerState 함수를 포함시켜 OnPlayerStateChanged를 호출하면 포함시킨 UpdatePlayerState 함수 작동
	PlayerState->OnPlayerStateChanged.AddUObject(this, &UABHUDWidget::UpdatePlayerState);
}

void UABHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();
	HPBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("pbHP")));
	ABCHECK(nullptr != HPBar);

	ExpBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("pbExp")));
	ABCHECK(nullptr != ExpBar);

	PlayerName = Cast<UTextBlock>(GetWidgetFromName(TEXT("txtPlayerName")));
	ABCHECK(nullptr != PlayerName);

	PlayerLevel = Cast<UTextBlock>(GetWidgetFromName(TEXT("txtLevel")));
	ABCHECK(nullptr != PlayerLevel);

	CurrentScore = Cast<UTextBlock>(GetWidgetFromName(TEXT("txtCurrentScore")));
	ABCHECK(nullptr != CurrentScore);

	HighScore = Cast<UTextBlock>(GetWidgetFromName(TEXT("txtHighScore")));
	ABCHECK(nullptr != HighScore);
}

void UABHUDWidget::UpdateCharacterStat()
{
	ABCHECK(CurrentCharacterStat.IsValid());

	HPBar->SetPercent(CurrentCharacterStat->GetHPRatio());
}

void UABHUDWidget::UpdatePlayerState()
{
	ABCHECK(CurrentPlayerState.IsValid());

	// 경험치 저장 및 레벨변화 534p
	ExpBar->SetPercent(CurrentPlayerState->GetExpRatio());
	//
	PlayerName->SetText(FText::FromString(CurrentPlayerState->GetPlayerName()));
	PlayerLevel->SetText(FText::FromString(FString::FromInt(CurrentPlayerState->GetCharacterLevel())));
	CurrentScore->SetText(FText::FromString(FString::FromInt(CurrentPlayerState->GetGameScore())));

	// 559p 저장된 데이터 HighScore 출력
	HighScore->SetText(FText::FromString(FString::FromInt(CurrentPlayerState->GetGameHighScroe())));
}
