// Fill out your copyright notice in the Description page of Project Settings.


#include "ABGameMode.h"
//#include "ABPawn.h" // ABPawn 말고 Pawn을 상속받은 Characher에서 오브젝트 설정
#include "ABCharacter.h"
#include "ABPlayerController.h"

// 플레이어 스탯정보 해더파일 14장 518p
#include "ABPlayerState.h"

AABGameMode::AABGameMode()
{
	//에디터의 월드셋팅에 선택한 GameMode 설정을 보면 변경된걸 볼 수 있음.
	// 기존 캐릭터를 AABPawn으로 설정
	//DefaultPawnClass = AABPawn::StaticClass();
	// 기존 캐릭터를 ABCharacter으로 설정
	DefaultPawnClass = AABCharacter::StaticClass();
	//기본 playercontroller 속성을 ABPlayerController로 변경
	PlayerControllerClass = AABPlayerController::StaticClass();

	//만약 폰을 블루 프린트로 제작된 폰을 기본 폰으로 사용하고자 할경우 접미사 _C 경로를 사용하면 블루프린트 에셋 사용
	//맨마지막에 _C는 직접 붙인거
	// 마지막에 DefaultPawnClass를 AAPawn 에서 BP_PAWN_C로 바꿨기 때문에 AAPawn의 ABLOG가 호출이 안되는걸로 추측
	// 중간에 호출되는 것이 아니라 AABGameMode의 코드를 전부 호출된 후에 DefaultPawnClass를 확인을 거쳐 셋팅되는 시간순서로 추측
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

	// AABPlayerState 로 PlayerState 변수를 다운캐스팅을 한다.
	// AABPlayerState는 APlayerState 상속받으며, PlayerState는 APlayerState의 클래스 변수 이므로 PlayState는 클래스는 AABPlayerState의 상위 클래스 이므로
	// PlayerState는 변수의 클래스는 AABPlayState로 다운캐스팅을 한다.
	// 만약 PlayerState의 클래스는 AABPlayerState의 상위 클래스가 아니라면 Cast는 실패가 되어 auto로 설정된 ABPlayState에는 nullptr을 가지게된다.
	auto ABPlayerState = Cast<AABPlayerState>(NewPlayer->PlayerState);
	ABCHECK(nullptr != ABPlayerState);
	ABPlayerState->InitPlayerData();

}
