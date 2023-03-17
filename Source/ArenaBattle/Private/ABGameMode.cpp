 // Fill out your copyright notice in the Description page of Project Settings.


#include "ABGameMode.h"
//#include "ABPawn.h" // ABPawn 말고 Pawn을 상속받은 Characher에서 오브젝트 설정
#include "ABCharacter.h"
#include "ABPlayerController.h"

// 플레이어 스탯정보 해더파일 14장 518p
#include "ABPlayerState.h"
// 스코어 점수 543p
#include "ABGameState.h"

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
	// StaticClass 는 컴파일 타임에서 UClass 타입의 정보를 얻어오는 것이며, GetClass 는 런타임에서 실제 객체의 클래스를 조회할때 사용된다.
	//GameStateClass = AABGameMode::StaticClass(); // 에러 발생
	GameStateClass = AABGameState::StaticClass();
	ABLOG(Warning, TEXT("생성자 호출"));

	// 615p 추가 ( 맵이 COMPLATE가 되면 1 추가되는 형식으로 맵 2개가 COMPLATE 되면 미션 완료)
    // 맵 클리어 해야하는 갯수
	ScoreToClear = 10;
}

void AABGameMode::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	ABGameState = Cast<AABGameState>(GameState);
	ABLOG(Warning, TEXT("PostInitializeComponents 호출"));
}

void AABGameMode::PostLogin(APlayerController* NewPlayer)
{
	ABLOG(Warning, TEXT("PostLogin Begin"));
	Super::PostLogin(NewPlayer);

	// AABPlayerState 로 PlayerState 변수를 다운캐스팅을 한다.
	// AABPlayerState는 APlayerState 상속받으며, PlayerState는 APlayerState의 클래스 변수 이므로 PlayState는 클래스는 AABPlayerState의 상위 클래스 이므로
	// PlayerState는 변수의 클래스는 AABPlayState로 다운캐스팅을 한다.
	// 만약 PlayerState의 클래스는 AABPlayerState의 상위 클래스가 아니라면 Cast는 실패가 되어 auto로 설정된 ABPlayState에는 nullptr을 가지게된다.
	auto ABPlayerState = Cast<AABPlayerState>(NewPlayer->PlayerState);
	ABCHECK(nullptr != ABPlayerState);
	ABPlayerState->InitPlayerData();

	ABLOG(Warning, TEXT("PostLogin End"));
}




void AABGameMode::AddScore(AABPlayerController* ScoredPlayer)
{
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		const auto ABPlayerController = Cast<AABPlayerController>(It->Get());
		if ((nullptr != ABPlayerController) && (ScoredPlayer == ABPlayerController))
		{
			ABPlayerController->AddGameScore();
			break;
		}
	}
	// AABGameState::StaticClass() 을 통해 GameStateClass에 AABGameState 클래스를 할당하였고  GameState를 통해 AABGameState의 포인터를 얻어 ABGameState 변수에 할당
	// 오류가 난 이유는 GameStateClass에 AABGameMode 클래스를 담았고(생성자 에러참고) 
	// 이로인해 GameState는 AABGameState를 ABGmaeState에 담고 싶었지만 GameState 변수에는 GameMode 클래스 정보만 할당하고 있고 GameMode에 ABGameState에 정보가 없어
	// ABGameState에 대해 변수와 함수가 액세스를 할 수가 없어 AddGameScore를 호출할 수 없었다.

	ABGameState->AddGameScore();

	// 615p 코드 추가 (미션 완료 UI출력 관련)
	if (GetScore() >= ScoreToClear)
	{
		ABGameState->SetGameCleared();
		// FConstPawnIterator 는 APawn 클래스의 인스턴스를 순회하기 위한 상수 이터레이터
		// 현재 월드에 활성화된 APawn 클래스 목록을 가져온다.
		// 단. 상수이기 떄문에 각 정보값은 가져올 수 있지만 삭제나 수정은 불가능하다.
		for (FConstPawnIterator It = GetWorld()->GetPawnIterator(); It; ++It)
		{
			// 가져온 Pawn들 전부 비활성화
			(*It)->TurnOff();
		}
		// 모든 플레이어 컨트롤러를 가져와서 AABPlayerController 캐스팅 해서 ABPlayerController에 대입
		for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
		{
			const auto ABPlayerController = Cast<AABPlayerController>(It->Get());
			// AABPlayerController 클래스를 상속받은 컨트룰러는 플레이어(나) 밖에 없으니
			// 다른 컨트룰러는 Cast가 되지않아 nullptr 를 가지게 되고 플레이어(나)차례가 오면 ShowResultUI 호출 
			if (nullptr != ABPlayerController)
			{
				ABPlayerController->ShowResultUI();
			}
		}
	}

}

int32 AABGameMode::GetScore() const
{
	return ABGameState->GetTotalGameScore();
}
