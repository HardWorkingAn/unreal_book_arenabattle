// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ArenaBattle.h"

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ABPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class ARENABATTLE_API AABPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	AABPlayerController();

	// Actor 컴포넌트가 완전히 초기화된 이후 호출된다. 게임 플레이 중에만 호출
	virtual void PostInitializeComponents() override;
	// OnPossess 는 컨트룰러가 폰에 빙의
	virtual void OnPossess(APawn* aPawn) override;
	// 4.22버전 이후부터는 Possess는 오버라이드 를 할 수 없으며 OnPossess로 바뀜
	//virtual void Possess(APawn* aPawn) override;

	class UABHUDWidget* GetHUDWidget() const;
	// 경험치 저장 및 레벨변화 536p
	void NPCKill(class AABCharacter* KilledNPC) const;
	// 546p
	void AddGameScore() const;

	// 606p 게임중지 기능
	void ChangeInputMode(bool bGameMode = true);

	// 611p 결과값 UI 출력 키
	void ShowResultUI();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = UI)
	TSubclassOf<class UABHUDWidget> HUDWidgetClass;

	// 600p 게임 중지 기능 키 바인딩
	virtual void SetupInputComponent() override;

	// 606p 게임중지 기능
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = UI)
	TSubclassOf<class UABGamePlayWidget> MenuWidgetClass;

	// 611p 결과값 UI 출력
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = UI)
	TSubclassOf<class UABGamePlayResultWidget> ResultWidgetClass;

private:
	UPROPERTY()
	class UABHUDWidget* HUDWidget;

	// 경험치 저장 및 레벨변화 536p
	UPROPERTY()
	class AABPlayerState* ABPlayerState;

	// 606p 게임중지 기능
	UPROPERTY()
	class UABGamePlayWidget* MenuWidget;

	// 611p 결과값 UI 출력 
	UPROPERTY()
	class UABGamePlayResultWidget* ResultWidget;

	// 600p 게임 중지 기능 키 바인딩
	void OnGamePause();

	// 606p 게임중지 기능 변수
	FInputModeGameOnly GameInputMode;
	FInputModeUIOnly UIInputMode;
	
	
};
