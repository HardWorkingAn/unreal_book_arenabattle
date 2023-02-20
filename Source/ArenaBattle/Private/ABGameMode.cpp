// Fill out your copyright notice in the Description page of Project Settings.


#include "ABGameMode.h"
//#include "ABPawn.h" // ABPawn ���� Pawn�� ��ӹ��� Characher���� ������Ʈ ����
#include "ABCharacter.h"
#include "ABPlayerController.h"

// �÷��̾� �������� �ش����� 14�� 518p
#include "ABPlayerState.h"

AABGameMode::AABGameMode()
{
	//�������� ������ÿ� ������ GameMode ������ ���� ����Ȱ� �� �� ����.
	// ���� ĳ���͸� AABPawn���� ����
	//DefaultPawnClass = AABPawn::StaticClass();
	// ���� ĳ���͸� ABCharacter���� ����
	DefaultPawnClass = AABCharacter::StaticClass();
	//�⺻ playercontroller �Ӽ��� ABPlayerController�� ����
	PlayerControllerClass = AABPlayerController::StaticClass();

	//���� ���� ��� ����Ʈ�� ���۵� ���� �⺻ ������ ����ϰ��� �Ұ�� ���̻� _C ��θ� ����ϸ� �������Ʈ ���� ���
	//�Ǹ������� _C�� ���� ���ΰ�
	// �������� DefaultPawnClass�� AAPawn ���� BP_PAWN_C�� �ٲ�� ������ AAPawn�� ABLOG�� ȣ���� �ȵǴ°ɷ� ����
	// �߰��� ȣ��Ǵ� ���� �ƴ϶� AABGameMode�� �ڵ带 ���� ȣ��� �Ŀ� DefaultPawnClass�� Ȯ���� ���� ���õǴ� �ð������� ����
	/*
	static ConstructorHelpers::FClassFinder<APawn> BP_PAWN_C(TEXT("/Game/ThirdPersonBP/Blueprints/ThirdPersonCharacter.ThirdPersonCharacter_C"));
	if (BP_PAWN_C.Succeeded())
	{
		DefaultPawnClass = BP_PAWN_C.Class;
	}
	*/

	PlayerStateClass = AABPlayerState::StaticClass();
}

void AABGameMode::PostLogin(APlayerController* NewPlayer)
{
	ABLOG(Warning, TEXT("PostLogin Begin"));
	Super::PostLogin(NewPlayer);
	ABLOG(Warning, TEXT("PostLogin End"));

	// AABPlayerState �� PlayerState ������ �ٿ�ĳ������ �Ѵ�.
	// AABPlayerState�� APlayerState ��ӹ�����, PlayerState�� APlayerState�� Ŭ���� ���� �̹Ƿ� PlayState�� Ŭ������ AABPlayerState�� ���� Ŭ���� �̹Ƿ�
	// PlayerState�� ������ Ŭ������ AABPlayState�� �ٿ�ĳ������ �Ѵ�.
	// ���� PlayerState�� Ŭ������ AABPlayerState�� ���� Ŭ������ �ƴ϶�� Cast�� ���а� �Ǿ� auto�� ������ ABPlayState���� nullptr�� �����Եȴ�.
	auto ABPlayerState = Cast<AABPlayerState>(NewPlayer->PlayerState);
	ABCHECK(nullptr != ABPlayerState);
	ABPlayerState->InitPlayerData();

}
