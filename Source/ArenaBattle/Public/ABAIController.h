// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ArenaBattle.h"

#include "CoreMinimal.h"
#include "AIController.h"
#include "ABAIController.generated.h"

/**
 * 
 */
UCLASS()
class ARENABATTLE_API AABAIController : public AAIController
{
	GENERATED_BODY()

// ������ 3�ʸ��� �������� �̵��ϴ� ���
// �𸮾� �׺���̼��� �̵������� �������� �������� �������� GetRandomPointInNavigableRTadius �Լ��� �������� ���� �̵���Ű�� SimpleMoveToLocation �Լ� ����
public:
	AABAIController();
	// AI ������ġ �̵�
	virtual void OnPossess(APawn* InPawn) override;
	//virtual void OnUnPossess() override;

	//������ ���� Ű �߰� �Ѱ� ����
	static const FName HomePosKey;
	static const FName PatrolPosKey;

	static const FName TargetKey;

	// 14�� 509p
	void RunAI();
	void StopAI();
private:
	// AI ������ġ �̵�
	/*
	void OnRepeatTimer();
	FTimerHandle RePeatTimerHandle;
	float ReapeatInterval;
	*/
	// �����̺�� Ʈ�� �� ������ ��� AI�̵�
	UPROPERTY()
	class UBehaviorTree* BTAsset;

	UPROPERTY()
	class UBlackboardData* BBAsset;

};
