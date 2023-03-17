#include "ABPlayerController.h"

// 14장 523p
#include "ABHUDWidget.h"

// 데이터 엑터 저정후 UI 연동
#include "ABPlayerState.h"
// 532p 경험치 저장 및 레벨변화 
#include "ABCharacter.h"
// 606p 게임중지 기능
#include "ABGamePlayWidget.h"
// 611p 게임 결과 UI 기능
#include "ABGamePlayResultWidget.h"
// 620p 게임 정보 가져오기
#include "ABGameState.h"

AABPlayerController::AABPlayerController()
{
	// UI_HUD 객체생성 but 화면에 띄우는곳은 Begin에서 띄움
	// 블루프린트로 이용하기 때문에 마지막에 _C 붙여준다.
	static ConstructorHelpers::FClassFinder<UABHUDWidget> UI_HUD_C(TEXT("/Game/Book/UI/UI_HUD.UI_HUD_C"));
	if (UI_HUD_C.Succeeded())
	{
		HUDWidgetClass = UI_HUD_C.Class;
	}

	// 606p 게임중지 기능
	// ※ UI 블루프린트로 했으면 UI 부모클래스 설정하자!
	static ConstructorHelpers::FClassFinder<UABGamePlayWidget> UI_MENU_C(TEXT("/Game/Book/UI/UI_Menu.UI_Menu_C"));
	if (UI_MENU_C.Succeeded())
	{
		MenuWidgetClass = UI_MENU_C.Class;
	}

	// 612p 코드 추가 (Result 위젯 출력)
	static ConstructorHelpers::FClassFinder<UABGamePlayResultWidget> UI_RESULT_C(TEXT("/Game/Book/UI/UI_Result.UI_Result_C"));
	if (UI_RESULT_C.Succeeded())
	{
		ResultWidgetClass = UI_RESULT_C.Class;
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

void AABPlayerController::ChangeInputMode(bool bGameMode)
{
	if (bGameMode)
	{
		SetInputMode(GameInputMode);
		bShowMouseCursor = false;
	}
	else
	{
		SetInputMode(UIInputMode);
		bShowMouseCursor = true;
	}
}
// 611p 기본작성
void AABPlayerController::ShowResultUI()
{
	// 620p 코드 추가
	// ABGameState를 include 한 이유는 현재 게임정보(ex:Score)를 가져와 ResultWidget에 저장해서
	// ABGamePlayReultWidget 에서 스코어 값을 가져와 최종스코어를 UI로 출력하기 때문
	auto ABGameState = Cast<AABGameState>(UGameplayStatics::GetGameState(this));
	ABCHECK(nullptr != ABGameState);
	ResultWidget->BindGameState(ABGameState);
	//
	ResultWidget->AddToViewport();
	ChangeInputMode(false);
}

void AABPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// 607p 추가 및 변경
	// 해더에서 변수 추가후 ChangeInputMode 를 통해 변경됨
	//FInputModeGameOnly InputMode;
	//SetInputMode(InputMode);
	
	ChangeInputMode(true);

	// UI 띄우기
	HUDWidget = CreateWidget<UABHUDWidget>(this, HUDWidgetClass);
	// 607p 에서 변경 및 추가
	//HUDWidget->AddToViewport(); 
	ABCHECK(nullptr != HUDWidget);
	// AddToViewport는 화면에 표시되는 위젯의 순서 기본값이 0이고
	// 낮을수록 맨앞에 UI가 보인다.
	HUDWidget->AddToViewport(1);


	// 612p 코드 추가 (Result 위젯 출력)\
	// ShowResultUI 호출되면 AddToViewport() 를 통해 화면표시 되는거라 미리 생성해도 무관함
	ResultWidget = CreateWidget<UABGamePlayResultWidget>(this, ResultWidgetClass);
	ABCHECK(nullptr != ResultWidget);

	// 데이터 엑터 저정후 UI 연동
	//auto ABPlayerState = Cast<AABPlayerState>(PlayerState); // ABPlayerState는 h 파일에 class AABPlayerState* 형태로 추가

	ABPlayerState = Cast<AABPlayerState>(PlayerState);
	ABCHECK(nullptr != ABPlayerState);
	HUDWidget->BindPlayerState(ABPlayerState);
	ABPlayerState->OnPlayerStateChanged.Broadcast();

}

void AABPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	// Action 키 만들어둔 GamePause (M키) 누르면 함수 작동되게 바인드
	InputComponent->BindAction(TEXT("GamePause"), EInputEvent::IE_Pressed, this, &AABPlayerController::OnGamePause);
}

void AABPlayerController::OnGamePause()
{
	MenuWidget = CreateWidget<UABGamePlayWidget>(this, MenuWidgetClass);
	ABCHECK(nullptr != MenuWidget);
	MenuWidget->AddToViewport(3);

	SetPause(true);
	ChangeInputMode(false);
}
