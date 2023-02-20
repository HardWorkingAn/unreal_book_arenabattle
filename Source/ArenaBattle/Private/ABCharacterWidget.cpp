// Fill out your copyright notice in the Description page of Project Settings.


#include "ABCharacterWidget.h"

// stat�� ���� HP ������ �������� ���� include
#include "ABCharacterStatComponent.h"

// progressbar ������Ʈ�� ���� �ش�����
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
	NewCharacterStat->OnHPChanged.AddUObject(this, &UABCharacterWidget::UpdateHPWidget); // ������Ʈ �Լ� ���ε�
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
	// IsValid�� ������ �ݷ����� ���� ��� ������ ���� ���ų� nullptr�� �ƴ� ��� true�� ��ȯ
	// ��ȿ�ϴ� : IsValid�� ������ �ݷ����� ���� ��� ������ ���� ���ų� nullptr�� �ƴ� ��� true�� ��ȯ�ؿ� �̶��� ��ȿ�ϴٰ� �Ѵ�.
	if (CurrentCharacterStat.IsValid())
	{
		if (nullptr != HPProgressBar)
		{
			HPProgressBar->SetPercent(CurrentCharacterStat->GetHPRatio());
		}
	}
}
