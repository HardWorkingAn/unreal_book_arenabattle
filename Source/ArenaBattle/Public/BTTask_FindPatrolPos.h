// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_FindPatrolPos.generated.h"


/**
 * 
 */

// TaskNode를 쓰기 위해서는 Bulid.cs 에서  "GameplayTasks" 를 추가 해줘야 컴파일에서 오류가 발생하지 않는다.

UCLASS()
class ARENABATTLE_API UBTTask_FindPatrolPos : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTTask_FindPatrolPos();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
