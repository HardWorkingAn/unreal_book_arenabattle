// Fill out your copyright notice in the Description page of Project Settings.


#include "ABPlayerController.h"

// 14�� 523p
#include "ABHUDWidget.h"

// ������ ���� ������ UI ����
#include "ABPlayerState.h"

// ����ġ ���� �� ������ȭ 532p
#include "ABCharacter.h"

AABPlayerController::AABPlayerController()
{
	// UI_HUD ��ü���� but ȭ�鿡 ���°��� Begin���� ���
	// �������Ʈ�� �̿��ϱ� ������ �������� _C �ٿ��ش�.
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
	
	// UI ����
	HUDWidget = CreateWidget<UABHUDWidget>(this, HUDWidgetClass);
	HUDWidget->AddToViewport();
	
	// ������ ���� ������ UI ����
	//auto ABPlayerState = Cast<AABPlayerState>(PlayerState); // ABPlayerState�� h ���Ͽ� class AABPlayerState* ���·� �߰�

	ABPlayerState = Cast<AABPlayerState>(PlayerState);
	ABCHECK(nullptr != ABPlayerState);
	HUDWidget->BindPlayerState(ABPlayerState);
	ABPlayerState->OnPlayerStateChanged.Broadcast();
	
}

