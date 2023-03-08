// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ArenaBattle.h"

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ABCharacter.generated.h"

// 공격을 끝내어 태스크 종료 알리는 델리게이트 443p
DECLARE_MULTICAST_DELEGATE(FOnAttackEndDelegate);


UCLASS()
class ARENABATTLE_API AABCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AABCharacter();

	// 스테이트 머신 모델로 게임 진행단계 구분 501p
	void SetCharacterState(ECharacterState NewState);
	ECharacterState GetCharacterState() const;
	// 경험치 저장 및 레벨변화 532p
	int32 GetExp() const;

	//562p 무기 속성
	float GetFinalAttackRange() const;
	//567p 무기 공격 대미지 속성
	float GetFinalAttackDamage() const;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	//GTA 처럼 시점을 조작을 가능하게 하거나 아니면 디아블로 처럼 시점은 고정 설정 선택
	enum class EControlMode
	{
		GTA,
		DIABLO,
		// NPC일경우
		NPC
	};
	//void SetControlMode(int32 ControlMode);
	void SetControlMode(EControlMode ControlMode);
	EControlMode CurrentControlMode = EControlMode::GTA;
	FVector DirectionToMove = FVector::ZeroVector;

	float ArmLengthTo = 0.0f;
	FRotator ArmRotationTo = FRotator::ZeroRotator;
	float ArmLengthSpeed = 0.0f;
	float ArmRotationSpeed = 0.0f;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	// 델리게이트 설정을 위한 컴포넌트
	virtual void PostInitializeComponents() override;
	// 대미지 처리
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	// NPC 일 경우 속도 및 회전속도 조절
	virtual void PossessedBy(AController* NewController) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// 캐릭터와 카메라의 위치 계산 해주는 SpringArm
	UPROPERTY(VisibleAnyWhere, Category = Camera)
	USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnyWhere, Category = Camera)
	UCameraComponent* Camera;

	// 캐릭터 무기 장착 모델 
	UPROPERTY(VisibleAnyWhere, Category = Weapon)
	USkeletalMeshComponent* Weapon;
	//아이템 습득시 무기변경
	bool CanSetWeapon();
	void SetWeapon(class AABWeapon* NewWeapon);
	UPROPERTY(VisibleAnyWhere, Category = Weapon)
	class AABWeapon* CurrentWeapon;

	// 11장 게임 데이터 테이블( 각종 스탯데이터 ) 설정

	UPROPERTY(VisibleAnywhere, Category = Stat)
	class UABCharacterStatComponent* CharacterStat;

	// UI 체력바
	UPROPERTY(VisibleAnywhere, Category = UI)
	class UWidgetComponent* HPBarWidget;

	// 공격 종료 함수 및 델리게이트
	void Attack();  //Attack함수 public으로 옮김
	FOnAttackEndDelegate OnAttackEnd; //델리게이트 변수 선언


private:
	void UpDown(float NewAxisValue);
	void LeftRight(float NewAxisValue);
	void LookUp(float NewAxisValue);
	void Turn(float NewAxisValue);

	void ViewChange();
	//void Attack();
	
	// https://hyo-ue4study.tistory.com/287 델리게이트 참고 사이트
	// OnAttackMentageEnded 를 델리게이트로 등륵해 몽타주 재생이 끝나는 시점을 파악할 수 있도록 한다.
	// UABAnumInstance안의 OnMontageEnded에 바인딩할 함수
	// UFUNCTION() 매크로를 추가해 OnAttack... 델리게이트는 블루프린트의 함수와 연동할 수 있도록 설계 (블루프린트
	UFUNCTION()
	void OnAttackMentageEnded(UAnimMontage* Montage, bool BInterrupted);

	// 노티파이 모션 공격 모션 끊어놓은거 연결
	void AttackStartComboState();
	void AttackEndComboState();
	// 충돌설정 (콜리전)
	void AttackCheck();

	// 13장 468p 캐릭터 외형 ini 로드한거 랜덤으로 가져오기
	void OnAssetLoadCompleted();

private:
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	bool IsAttacking;

	// 여러곳에서 쓰이는 AnimInstance를 클래스변수화 하여 어느 함수에서든 사용가능하게 제작할 변수
	UPROPERTY()
	class UABAnimInstance* ABAnim;

	// 스테이트 머신 모델로 게임 진행단계 14장 구분 501p 
	int32 AssetIndex = 0;

	// 노티파이 모션 공격 모션 끊어놓은거 연결

	UPROPERTY(VisibleInstanceOnly, BluePrintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	bool CanNextCombo;
	UPROPERTY(VisibleInstanceOnly, BluePrintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	bool IsComboInputOn;
	UPROPERTY(VisibleInstanceOnly, BluePrintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	int32 CurrentCombo;
	UPROPERTY(VisibleInstanceOnly, BluePrintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	int32 MaxCombo;

	// DrawDebug
	UPROPERTY(VisibleInstanceOnly, BluePrintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	float AttackRange;
	UPROPERTY(VisibleInstanceOnly, BluePrintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	float AttackRadius;

	// 13장 468p
	FSoftObjectPath CharacterAssetToLoad = FSoftObjectPath(nullptr);
	TSharedPtr<struct FStreamableHandle> AssetStreamingHandle;

	// 스테이트 머신 모델로 게임 진행단계 14장 구분 501p 
	UPROPERTY(Transient, VisibleInstanceOnly, BlueprintReadOnly, Category = State, Meta = (AllowPrivateAccess = true))
	ECharacterState CurrentState;

	UPROPERTY(Transient, VisibleInstanceOnly, BlueprintReadOnly, Category = State, Meta = (AllowPrivateAccess = true))
	bool bIsPlayer;

	UPROPERTY()
	class  AABAIController* ABAIController;

	UPROPERTY()
	class AABPlayerController* ABPlayerController;


	// 14장 512p
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = State, Meta = (AllowPrivateAccess = true))
	float DeadTimer;

	FTimerHandle DeadTimerHandle = {};
};
