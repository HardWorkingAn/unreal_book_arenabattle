// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ArenaBattle.h"

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "ABAnimInstance.generated.h"

//��Ƽ���� ��� ����
DECLARE_MULTICAST_DELEGATE(FOnNextAttackCheckDelegate);
DECLARE_MULTICAST_DELEGATE(FOnAttackHitCheckDelegate);

/**
 * 
 */
UCLASS()
class ARENABATTLE_API UABAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	UABAnimInstance();
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	void PlayAttackMontage();
	
	//��Ƽ���� ��� ����
	void JumpToAttackMontageSection(int32 NewSection);

public:
	FOnNextAttackCheckDelegate OnNextAttackCheck;
	FOnAttackHitCheckDelegate OnAttackHitCheck;

	// ����� �ް� ����ϴ� �ִϸ��̼�
	void SetDeadAnim() { IsDead = true; }
	
// �ִϸ��̼��� ��Ʈ ����
private:
	// �𸮾� ��Ÿ���� ã�� �� �ֵ��� �ݵ�� UFUNCTION ��ũ�η� ����������Ѵ�.
	UFUNCTION()
	void AnimNotify_AttackHitCheck();
	//��Ƽ���� ��� ����
	UFUNCTION()
	void AnimNotify_NextAttackCheck();

	FName GetAttackMontageSectionName(int32 Section);
private:
	//��� ����Ʋ���� �����ϴ� Ű����� BlueprintReadOnly, BlueprintReadWrite �� ������ �ִ�.
	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category=Pawn, Meta=(AllowPrivateAccess=true))
	float CurrentPawnSpeed;

	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category=Pawn, Meta=(AllowPrivateAccess=true))
	bool IsInAir;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	UAnimMontage* AttackMontage;

	// ����� �ް� ����ϴ� �ִϸ��̼� (�Ӽ��߰�)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	bool IsDead;
};
