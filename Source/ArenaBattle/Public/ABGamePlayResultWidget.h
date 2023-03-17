// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ArenaBattle.h"

#include "CoreMinimal.h"
#include "ABGamePlayWidget.h"
#include "ABGamePlayResultWidget.generated.h"

/**
 * 
 */
UCLASS()
class ARENABATTLE_API UABGamePlayResultWidget : public UABGamePlayWidget
{
	GENERATED_BODY()

public:
	// 618p Result UI 버튼 바인드
	void BindGameState(class AABGameState* GameState);

protected:
	virtual void NativeConstruct() override;

private:
	// 618p Result UI 버튼 바인드
	TWeakObjectPtr<class AABGameState> CurrentGameState;



};
