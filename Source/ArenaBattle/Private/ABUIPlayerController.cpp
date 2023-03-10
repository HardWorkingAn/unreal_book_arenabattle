// Fill out your copyright notice in the Description page of Project Settings.


#include "ABUIPlayerController.h"

// 577p 추가
#include "Blueprint/UserWidget.h"

void AABUIPlayerController::BeginPlay()
{
	Super::BeginPlay();

	ABCHECK(nullptr != UIWidgetClass);

	UIWidgetInstance = CreateWidget<UUserWidget>(this, UIWidgetClass);
	ABCHECK(nullptr != UIWidgetInstance);

	UIWidgetInstance->AddToViewport();

	// UI를 위해 사용되는 입력 모드 중 하나
	// 게임 뷰포트에서 UI 요소를 클릭하고 사용할 수 있도록 입력을 UI로만 제한
	// 게임 뷰포트에서 마우스 및 키보드 입력을 처리하지 않고 대신 UI 위젯에서 처리
	// 예를 들어 게임 옵션을 켰는데 체크할때 클릭이 UI 외에는 클릭 판정을 막음
	FInputModeUIOnly Mode;
	Mode.SetWidgetToFocus(UIWidgetInstance->GetCachedWidget());
	SetInputMode(Mode);
	// 마우스 커서 보이게 true
	bShowMouseCursor = true;

}
