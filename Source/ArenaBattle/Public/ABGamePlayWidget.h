// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ArenaBattle.h"

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ABGamePlayWidget.generated.h"

/**
 * 
 */

// 게임 중지 UI  UI_Result랑 연동
UCLASS()
class ARENABATTLE_API UABGamePlayWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnResumeClicked();

	UFUNCTION()
	void OnReturnToTitleClicked();

	UFUNCTION()
	void OnRetryGameClicked();

protected:

	UPROPERTY()
	class UButton* ResumeButton;

	UPROPERTY()
	class UButton* ReturnToTitleButton;

	UPROPERTY()
	class UButton* RetryGameButton;
};
