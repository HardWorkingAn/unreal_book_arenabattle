// Fill out your copyright notice in the Description page of Project Settings.


#include "ABAIController.h"

// 상위버전에 대한 변경사항
#include "NavigationSystem.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"

// 비헤이비어 트리 및 블랙보드 사용 AI이동
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackBoardData.h"

// 블랙보드에서 키 추가한거 연결
#include "BehaviorTree/BlackboardComponent.h"

const FName AABAIController::HomePosKey(TEXT("HomePos"));
const FName AABAIController::PatrolPosKey(TEXT("PatrolPos"));

const FName AABAIController::TargetKey(TEXT("Target"));

AABAIController::AABAIController()
{
	// ReapeatInterval = 3.0f;

	// 비헤이비어 트리 및 블랙보드 사용 AI이동
	static ConstructorHelpers::FObjectFinder<UBlackboardData> BBObject(TEXT("BlackboardData'/Game/Book/AI/BB_ABCharacter.BB_ABCharacter'"));
	if (BBObject.Succeeded())
	{
		BBAsset = BBObject.Object;
	}

	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTObject(TEXT("BehaviorTree'/Game/Book/AI/BT_ABCharacter.BT_ABCharacter'"));
	if (BTObject.Succeeded())
	{
		BTAsset = BTObject.Object;
	}
}

void AABAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	//GetWorld()->GetTimerManager().SetTimer(RePeatTimerHandle, this, &AABAIController::OnRepeatTimer, ReapeatInterval, true);

	// 14장 510p RunAI 에서 작동
	/*
	if (UseBlackboard(BBAsset, Blackboard))
	{
		// 블랙보드 키 연결한거 추가
		Blackboard->SetValueAsVector(HomePosKey, InPawn->GetActorLocation());
		//
		if (!RunBehaviorTree(BTAsset))
		{
			ABLOG(Error, TEXT("AIController couldn't run behvior tree"));
		}
	}
	*/
}
void AABAIController::RunAI()
{
	if (UseBlackboard(BBAsset, Blackboard))
	{
		// 블랙보드 키 연결한거 추가
		Blackboard->SetValueAsVector(HomePosKey, GetPawn()->GetActorLocation());
		// 
		if (!RunBehaviorTree(BTAsset))
		{
			ABLOG(Error, TEXT("AIController couldn't run behvior tree"));
		}
	}
}
void AABAIController::StopAI()
{
	auto BehiviorTreeComponent = Cast<UBehaviorTreeComponent>(BrainComponent);
	if (nullptr != BehiviorTreeComponent)
	{
		BehiviorTreeComponent->StopTree(EBTStopMode::Safe);
	}
}
// AI 랜덤위치 이동
/*
void AABAIController::OnUnPossess()
{
	Super::OnUnPossess();
	GetWorld()->GetTimerManager().ClearTimer(RePeatTimerHandle);
}
void AABAIController::OnRepeatTimer()
{
	auto CurrentPawn = GetPawn();
	ABCHECK(nullptr != CurrentPawn);
	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(GetWorld());
	if(nullptr == NavSystem) return;
	FNavLocation NextLocation;
	if (NavSystem->GetRandomPointInNavigableRadius(FVector::ZeroVector, 500.0f, NextLocation))
	{
		UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, NextLocation.Location);
		ABLOG(Warning, TEXT("Next Location %s"), *NextLocation.Location.ToString());
	}
}
*/
