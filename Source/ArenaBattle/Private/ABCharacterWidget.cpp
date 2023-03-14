// Fill out your copyright notice in the Description page of Project Settings.


#include "ABCharacterWidget.h"

// stat의 현제 HP 정보를 가져오기 위한 include
#include "ABCharacterStatComponent.h"

// progressbar 업데이트를 위한 해더파일
#include "Components/ProgressBar.h"

void UABCharacterWidget::BindCharacterStat(class UABCharacterStatComponent* NewCharacterStat)
{
	ABCHECK(nullptr != NewCharacterStat);

	CurrentCharacterStat = NewCharacterStat;
	NewCharacterStat->OnHPChanged.AddLambda([this]() -> void {

		if (CurrentCharacterStat.IsValid())
		{
			ABLOG(Warning, TEXT("HPRatio : %f"), CurrentCharacterStat->GetHPRatio());
		}

		});
	NewCharacterStat->OnHPChanged.AddUObject(this, &UABCharacterWidget::UpdateHPWidget); // 업데이트 함수 바인딩
}

void UABCharacterWidget::NativeConstruct()
{
	Super::NativeConstruct();
	HPProgressBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("PB_HPBar")));
	ABCHECK(nullptr != HPProgressBar);
	UpdateHPWidget();
}

void UABCharacterWidget::UpdateHPWidget()
{
	// IsValid는 가비지 콜렉터의 제거 대상 영역인 곳에 없거나 nullptr이 아닌 경우 true를 반환
	// 유효하다 : IsValid는 가비지 콜렉터의 제거 대상 영역인 곳에 없거나 nullptr이 아닌 경우 true를 반환해용 이때를 유효하다고 한다.
	if (CurrentCharacterStat.IsValid())
	{
		if (nullptr != HPProgressBar)
		{
			HPProgressBar->SetPercent(CurrentCharacterStat->GetHPRatio());
		}
	}
}
