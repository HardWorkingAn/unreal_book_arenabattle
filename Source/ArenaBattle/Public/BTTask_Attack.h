// Fill out your copyright notice in the Description page of Project Settings.

// 441p 태스크 공격명령

#pragma once

#include "ArenaBattle.h"

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_Attack.generated.h"

/**
 * 
 */
UCLASS()
class ARENABATTLE_API UBTTask_Attack : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
    UBTTask_Attack();

    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
    virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
    bool IsAttacking = false;
};
