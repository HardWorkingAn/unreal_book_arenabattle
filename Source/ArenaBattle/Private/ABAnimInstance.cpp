// Fill out your copyright notice in the Description page of Project Settings.


#include "ABAnimInstance.h"
#include "GameFrameWork/Character.h"
#include "GameFrameWork/PawnMovementComponent.h"


UABAnimInstance::UABAnimInstance()
{
	CurrentPawnSpeed = 0.0f;
	// IsInAir ���߿��� ������ true �ƴϸ� fasle
	IsInAir = false;
	// ����� �ޱ��� ����ִ� ���� bool �� ����
	IsDead = false;

	static ConstructorHelpers::FObjectFinder<UAnimMontage> ATTACK_MONTAGE(TEXT("/Game/Book/animations/SK_Mannequin_Skeleton_Montage.SK_Mannequin_Skeleton_Montage"));
	if (ATTACK_MONTAGE.Succeeded())
	{
		AttackMontage = ATTACK_MONTAGE.Object;
	}
}

void UABAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	// TryGetPawnOwner�� �����ܰ迡 ���� �����ϰ� �ִϸ��̼��� �����Ѵٸ� ��ȿ���� �ʴ� �� ��ü�� �����ϰ� �ǹǷ�
	// �ִϸ��̼� �ý����� ���� ������ �� �� ��ü�� ��ȣ���� �����ϴ� �Լ�
	auto Pawn = TryGetPawnOwner();
	/*
	if (::IsValid(Pawn))
	{
		CurrentPawnSpeed = Pawn->GetVelocity().Size();
		auto Character = Cast<ACharacter>(Pawn);
		if (Character)
		{
			// IsFalling Ű����� ���� ���߿� ���ִ���, �����ϰ� �ִ���(���� �������� �ִ°�?(�ٴڿ� �����ʾҴ°�?)) �˷��ִ� Ű���� �����ϴ����̶�� true �ƴϸ� false 
			// IsSwimming - �����ϴ��� Ȯ��  IsCrouching - �ޱ���(�ɾ�) �ִ��� Ȯ�� IsMoveOnGround - �� ������ �̵������� Ȯ��
			//IsInAir = Character->GetMovementComponent()->IsFalling();
			IsInAir = Character->GetMovementComponent()->IsFalling();
		}	
	}
	*/
	if (!::IsValid(Pawn)) return;
	if (!IsDead) // ����ִٸ� IsDead�� false���
	{
		CurrentPawnSpeed = Pawn->GetVelocity().Size();
		auto Character = Cast<ACharacter>(Pawn);
		if (Character)
		{
			// IsFalling Ű����� ���� ���߿� ���ִ���, �����ϰ� �ִ���(���� �������� �ִ°�?(�ٴڿ� �����ʾҴ°�?)) �˷��ִ� Ű���� �����ϴ����̶�� true �ƴϸ� false 
			// IsSwimming - �����ϴ��� Ȯ��  IsCrouching - �ޱ���(�ɾ�) �ִ��� Ȯ�� IsMoveOnGround - �� ������ �̵������� Ȯ��
			//IsInAir = Character->GetMovementComponent()->IsFalling();
			IsInAir = Character->GetMovementComponent()->IsFalling();
		}
	}
}

void UABAnimInstance::PlayAttackMontage()
{
	// ��������Ʈ�� ���� ���۰� ���ᰡ �����ǹǷ� ���� Montage_IsPlaying�� ������� �ʾƵ� �ȴ�.	
	//if (!Montage_IsPlaying(AttackMontage))
	//{
	//	Montage_Play(AttackMontage, 1.0f);
	//}
	//
	ABCHECK(!IsDead);
	Montage_Play(AttackMontage, 1.0f);
}



// ��Ƽ���� ��� ����
void UABAnimInstance::JumpToAttackMontageSection(int32 NewSection)
{
	ABCHECK(!IsDead);
	ABCHECK(Montage_IsPlaying(AttackMontage));
	Montage_JumpToSection(GetAttackMontageSectionName(NewSection), AttackMontage);
}
// ���� ��� üũ
void UABAnimInstance::AnimNotify_AttackHitCheck()
{
	OnAttackHitCheck.Broadcast();
}

void UABAnimInstance::AnimNotify_NextAttackCheck()
{
	OnNextAttackCheck.Broadcast();
}


FName UABAnimInstance::GetAttackMontageSectionName(int32 Section)
{
	ABCHECK(FMath::IsWithinInclusive<int32>(Section, 1, 4), NAME_None);
	return FName(*FString::Printf(TEXT("Attack%d"), Section));
}
