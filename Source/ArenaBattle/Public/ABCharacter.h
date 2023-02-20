// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ArenaBattle.h"

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ABCharacter.generated.h"

// ������ ������ �½�ũ ���� �˸��� ��������Ʈ 443p
DECLARE_MULTICAST_DELEGATE(FOnAttackEndDelegate);


UCLASS()
class ARENABATTLE_API AABCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AABCharacter();

	// ������Ʈ �ӽ� �𵨷� ���� ����ܰ� ���� 501p
	void SetCharacterState(ECharacterState NewState);
	ECharacterState GetCharacterState() const;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	//GTA ó�� ������ ������ �����ϰ� �ϰų� �ƴϸ� ��ƺ�� ó�� ������ ���� ���� ����
	enum class EControlMode
	{
		GTA,
		DIABLO,
		// NPC�ϰ��
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
	// ��������Ʈ ������ ���� ������Ʈ
	virtual void PostInitializeComponents() override;
	// ����� ó��
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	// NPC �� ��� �ӵ� �� ȸ���ӵ� ����
	virtual void PossessedBy(AController* NewController) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// ĳ���Ϳ� ī�޶��� ��ġ ��� ���ִ� SpringArm
	UPROPERTY(VisibleAnyWhere, Category = Camera)
	USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnyWhere, Category = Camera)
	UCameraComponent* Camera;

	// ĳ���� ���� ���� �� 
	UPROPERTY(VisibleAnyWhere, Category = Weapon)
	USkeletalMeshComponent* Weapon;
	//������ ����� ���⺯��
	bool CanSetWeapon();
	void SetWeapon(class AABWeapon* NewWeapon);
	UPROPERTY(VisibleAnyWhere, Category = Weapon)
	class AABWeapon* CurrentWeapon;

	// 11�� ���� ������ ���̺�( ���� ���ȵ����� ) ����

	UPROPERTY(VisibleAnywhere, Category = Stat)
	class UABCharacterStatComponent* CharacterStat;

	// UI ü�¹�
	UPROPERTY(VisibleAnywhere, Category = UI)
	class UWidgetComponent* HPBarWidget;

	// ���� ���� �Լ� �� ��������Ʈ
	void Attack();  //Attack�Լ� public���� �ű�
	FOnAttackEndDelegate OnAttackEnd; //��������Ʈ ���� ����


private:
	void UpDown(float NewAxisValue);
	void LeftRight(float NewAxisValue);
	void LookUp(float NewAxisValue);
	void Turn(float NewAxisValue);

	void ViewChange();
	//void Attack();
	
	// https://hyo-ue4study.tistory.com/287 ��������Ʈ ���� ����Ʈ
	// OnAttackMentageEnded �� ��������Ʈ�� ��� ��Ÿ�� ����� ������ ������ �ľ��� �� �ֵ��� �Ѵ�.
	// UABAnumInstance���� OnMontageEnded�� ���ε��� �Լ�
	// UFUNCTION() ��ũ�θ� �߰��� OnAttack... ��������Ʈ�� �������Ʈ�� �Լ��� ������ �� �ֵ��� ���� (�������Ʈ
	UFUNCTION()
	void OnAttackMentageEnded(UAnimMontage* Montage, bool BInterrupted);

	// ��Ƽ���� ��� ���� ��� ��������� ����
	void AttackStartComboState();
	void AttackEndComboState();
	// �浹���� (�ݸ���)
	void AttackCheck();

	// 13�� 468p ĳ���� ���� ini �ε��Ѱ� �������� ��������
	void OnAssetLoadCompleted();

private:
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	bool IsAttacking;

	// ���������� ���̴� AnimInstance�� Ŭ��������ȭ �Ͽ� ��� �Լ������� ��밡���ϰ� ������ ����
	UPROPERTY()
	class UABAnimInstance* ABAnim;

	// ������Ʈ �ӽ� �𵨷� ���� ����ܰ� 14�� ���� 501p 
	int32 AssetIndex = 0;

	// ��Ƽ���� ��� ���� ��� ��������� ����

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

	// 13�� 468p
	FSoftObjectPath CharacterAssetToLoad = FSoftObjectPath(nullptr);
	TSharedPtr<struct FStreamableHandle> AssetStreamingHandle;

	// ������Ʈ �ӽ� �𵨷� ���� ����ܰ� 14�� ���� 501p 
	UPROPERTY(Transient, VisibleInstanceOnly, BlueprintReadOnly, Category = State, Meta = (AllowPrivateAccess = true))
	ECharacterState CurrentState;

	UPROPERTY(Transient, VisibleInstanceOnly, BlueprintReadOnly, Category = State, Meta = (AllowPrivateAccess = true))
	bool bIsPlayer;

	UPROPERTY()
	class  AABAIController* ABAIController;

	UPROPERTY()
	class AABPlayerController* ABPlayerController;


	// 14�� 512p
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = State, Meta = (AllowPrivateAccess = true))
	float DeadTimer;

	FTimerHandle DeadTimerHandle = {};
};
