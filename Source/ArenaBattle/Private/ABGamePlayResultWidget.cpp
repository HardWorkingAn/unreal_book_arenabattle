// Fill out your copyright notice in the Description page of Project Settings.


#include "ABGamePlayResultWidget.h"
#include "Components/TextBlock.h"

// 618p Result UI버튼 바인딩 및 btnRetryGame 버튼 기능 구현
#include "ABGameState.h"

void UABGamePlayResultWidget::BindGameState(class AABGameState* GameState)
{
	ABCHECK(nullptr != GameState);
	CurrentGameState = GameState;
}

// 기본작성 611p
void UABGamePlayResultWidget::NativeConstruct()
{
	Super::NativeConstruct();
	// 619 코드 추가
	ABCHECK(nullptr != CurrentGameState);
	//
	auto Result = Cast<UTextBlock>(GetWidgetFromName(TEXT("txtResult")));
	ABCHECK(nullptr != Result);
	// 619 코드 추가
	Result->SetText(FText::FromString(CurrentGameState->IsGameCleared() ?
		TEXT("Mission Complate") : TEXT("Mission Failed")));
	//
	auto TotalScore = Cast<UTextBlock>(GetWidgetFromName(TEXT("txtTotalScore")));
	ABCHECK(nullptr != TotalScore);
	// 619 코드 추가
	TotalScore->SetText(FText::FromString(FString::FromInt(CurrentGameState->GetTotalGameScore())));

}

