// Fill out your copyright notice in the Description page of Project Settings.


#include "ABAnimInstance.h"
#include "GameFrameWork/Character.h"
#include "GameFrameWork/PawnMovementComponent.h"


UABAnimInstance::UABAnimInstance()
{
	CurrentPawnSpeed = 0.0f;
	// IsInAir 공중에서 있으면 true 아니면 fasle
	IsInAir = false;
	// 대미지 받기전 살아있는 상태 bool 값 설정
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

	// TryGetPawnOwner는 로직단계에 폰을 제거하고 애니메이션을 실행한다면 유효하지 않는 폰 객체를 참조하게 되므로
	// 애니메이션 시스템은 폰에 접근할 때 폰 객체가 유호한지 점검하는 함수
	auto Pawn = TryGetPawnOwner();
	/*
	if (::IsValid(Pawn))
	{
		CurrentPawnSpeed = Pawn->GetVelocity().Size();
		auto Character = Cast<ACharacter>(Pawn);
		if (Character)
		{
			// IsFalling 키워드는 현제 공중에 떠있는지, 낙하하고 있는지(현재 떨어지고 있는가?(바닥에 닿지않았는가?)) 알려주는 키워드 낙하하는중이라면 true 아니면 false
			// IsSwimming - 수영하는지 확인  IsCrouching - 쭈구려(앉아) 있는지 확인 IsMoveOnGround - 땅 위에서 이동중인지 확인
			//IsInAir = Character->GetMovementComponent()->IsFalling();
			IsInAir = Character->GetMovementComponent()->IsFalling();
		}
	}
	*/
	if (!::IsValid(Pawn)) return;
	if (!IsDead) // 살아있다면 IsDead가 false라면
	{
		CurrentPawnSpeed = Pawn->GetVelocity().Size();
		auto Character = Cast<ACharacter>(Pawn);
		if (Character)
		{
			// IsFalling 키워드는 현제 공중에 떠있는지, 낙하하고 있는지(현재 떨어지고 있는가?(바닥에 닿지않았는가?)) 알려주는 키워드 낙하하는중이라면 true 아니면 false 
			// IsSwimming - 수영하는지 확인  IsCrouching - 쭈구려(앉아) 있는지 확인 IsMoveOnGround - 땅 위에서 이동중인지 확인
			//IsInAir = Character->GetMovementComponent()->IsFalling();
			IsInAir = Character->GetMovementComponent()->IsFalling();
		}
	}
}

void UABAnimInstance::PlayAttackMontage()
{
	// 델리게이트에 의해 시작과 종료가 감지되므로 굳이 Montage_IsPlaying을 사용하지 않아도 된다.	
	//if (!Montage_IsPlaying(AttackMontage))
	//{
	//	Montage_Play(AttackMontage, 1.0f);
	//}
	//
	ABCHECK(!IsDead);
	Montage_Play(AttackMontage, 1.0f);
}



// 노티파이 모션 연결
void UABAnimInstance::JumpToAttackMontageSection(int32 NewSection)
{
	ABCHECK(!IsDead);
	ABCHECK(Montage_IsPlaying(AttackMontage));
	Montage_JumpToSection(GetAttackMontageSectionName(NewSection), AttackMontage);
}
// 공격 모션 체크
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
