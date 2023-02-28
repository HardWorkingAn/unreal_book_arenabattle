// Fill out your copyright notice in the Description page of Project Settings.


#include "ABPlayerController.h"

// 14장 523p
#include "ABHUDWidget.h"

// 데이터 엑터 저정후 UI 연동
#include "ABPlayerState.h"

// 경험치 저장 및 레벨변화 532p
#include "ABCharacter.h"

AABPlayerController::AABPlayerController()
{
	// UI_HUD 객체생성 but 화면에 띄우는곳은 Begin에서 띄움
	// 블루프린트로 이용하기 때문에 마지막에 _C 붙여준다.
	static ConstructorHelpers::FClassFinder<UABHUDWidget> UI_HUD_C(TEXT("/Game/Book/UI/UI_HUD.UI_HUD_C"));
	if (UI_HUD_C.Succeeded())
	{
		HUDWidgetClass = UI_HUD_C.Class;
	}
}

void AABPlayerController::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	ABLOG_S(Warning);
}

void AABPlayerController::OnPossess(APawn* aPawn)
{
	ABLOG_S(Warning);
	Super::OnPossess(aPawn);

	
}

UABHUDWidget* AABPlayerController::GetHUDWidget() const
{
	return HUDWidget;
}

void AABPlayerController::NPCKill(AABCharacter* KilledNPC) const
{
	ABPlayerState->AddExp(KilledNPC->GetExp());
}

void AABPlayerController::AddGameScore() const
{
	ABPlayerState->AddGameScore();
}

void AABPlayerController::BeginPlay()
{
	Super::BeginPlay();

	FInputModeGameOnly InputMode;
	SetInputMode(InputMode);
	
	// UI 띄우기
	HUDWidget = CreateWidget<UABHUDWidget>(this, HUDWidgetClass);
	HUDWidget->AddToViewport();
	
	// 데이터 엑터 저정후 UI 연동
	//auto ABPlayerState = Cast<AABPlayerState>(PlayerState); // ABPlayerState는 h 파일에 class AABPlayerState* 형태로 추가

	ABPlayerState = Cast<AABPlayerState>(PlayerState);
	ABCHECK(nullptr != ABPlayerState);
	HUDWidget->BindPlayerState(ABPlayerState);
	ABPlayerState->OnPlayerStateChanged.Broadcast();
	
}

