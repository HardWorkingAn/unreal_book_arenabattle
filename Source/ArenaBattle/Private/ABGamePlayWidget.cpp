// Fill out your copyright notice in the Description page of Project Settings.


#include "ABGamePlayWidget.h"

#include "Components/Button.h"
#include "ABPlayerController.h"

void UABGamePlayWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// UI 버튼 과 C++ 변수와 연동
	ResumeButton = Cast<UButton>(GetWidgetFromName(TEXT("btnResume")));
	if (nullptr != ResumeButton)
	{
		// 클릭 델릭게이트 등륵
		ResumeButton->OnClicked.AddDynamic(this, &UABGamePlayWidget::OnResumeClicked);
	}

	ReturnToTitleButton = Cast<UButton>(GetWidgetFromName(TEXT("btnReturnToTitle")));
	if (nullptr != ReturnToTitleButton)
	{
		// 클릭 델릭게이트 등륵
		ReturnToTitleButton->OnClicked.AddDynamic(this, &UABGamePlayWidget::OnReturnToTitleClicked);
	}

	RetryGameButton = Cast<UButton>(GetWidgetFromName(TEXT("btnRetryGame")));
	if (nullptr != RetryGameButton)
	{
		// 클릭 델릭게이트 등륵
		RetryGameButton->OnClicked.AddDynamic(this, &UABGamePlayWidget::OnRetryGameClicked);
	}

}
// 기본코드 608p
void UABGamePlayWidget::OnResumeClicked()
{
	auto ABPlayerController = Cast<AABPlayerController>(GetOwningPlayer());
	ABCHECK(nullptr != ABPlayerController);

	RemoveFromParent();

	ABPlayerController->ChangeInputMode(true);
	ABPlayerController->SetPause(false);
}
// 기본코드 608p
void UABGamePlayWidget::OnReturnToTitleClicked()
{
	UGameplayStatics::OpenLevel(GetWorld(), TEXT("Title"));
}
// 기본코드 619p
void UABGamePlayWidget::OnRetryGameClicked()
{
	auto ABPlayerController = Cast<AABPlayerController>(GetOwningPlayer());
	ABCHECK(nullptr != ABPlayerController);
	ABPlayerController->RestartLevel();
}
