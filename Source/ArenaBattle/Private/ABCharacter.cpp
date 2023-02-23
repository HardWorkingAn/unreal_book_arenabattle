// Fill out your copyright notice in the Description page of Project Settings.


#include "ABCharacter.h"

#include "ABAnimInstance.h"
// DrawDebug
#include "DrawDebugHelpers.h"

// ���� ������ ���� �ش����� begin ���� ����
#include "ABWeapon.h"

// ������ ����
#include "ABCharacterStatComponent.h"

// UI ����
#include "Components/WidgetComponent.h"
#include "ABCharacterWidget.h"

// AI ��Ʈ�ѷ� 
#include "ABAIController.h"

// INI���� �о����
#include "ABCharacterSetting.h"

// 13�� a
#include "ABGameInstance.h"

// 14��
#include "ABPlayerController.h"
// �÷��̾� ������(����)�� UI�����ϱ� ���� �ش� ex) �������� , �̸�(�г���), ����
#include "ABPlayerState.h"
#include "ABHUDWidget.h"

// ĳ���� ������Ʈ -> ���� -> �� -> ĳ���� ������ �����  ������Ʈ�� Root�θ�
// ĳ���ʹ� APawn���� �� �� ������ �ִϸ��̼��� ���� �Ļ��� Ŭ��������Ѵ�. 
// ������� �����ϰ� �����ϰ�, �ϴ��� ���ƴٴϴ� ���ϸ��̼ǵ�� �� ���ϸ��̼ǵ��� �����ϴ� �۾� ��� ������ ���ϸ��̼��� ó���� �� �ֵ��� ���
// ĳ���Ϳ��� ������Ʈ �����ϰ� InputComponent���� Input ���� ����
// Sets default values
AABCharacter::AABCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SPRINGARM"));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CAMERA"));
	// UI �����߰�
	HPBarWidget = CreateDefaultSubobject<UWidgetComponent>("HPBARWIDGET");
	
	SpringArm->SetupAttachment(GetCapsuleComponent());
	Camera->SetupAttachment(SpringArm);
	// UI �����߰� 
	HPBarWidget->SetupAttachment(GetMesh());

	//Pawn������ �׳� Mesh->�� ������ ���⼭�� GetMesh()-> �����
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -88.0f), FRotator(0.0f, -90.0f, 0.0f));
	SpringArm->TargetArmLength = 400.0f;
	SpringArm->SetRelativeRotation(FRotator(-15.0f, 0.0f, 0.0f));

	// 11�� ���� ������ ���̺�( ���� ���ȵ����� ) ����
	// cpp ���Ͽ��� include �߰�
	CharacterStat = CreateDefaultSubobject<UABCharacterStatComponent>(TEXT("CHARACTERSTAT"));

	//�� � �� ���� Object�� ���
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_CARDBOARD(TEXT("/Game/InfinityBladeWarriors/Character/CompleteCharacters/SK_CharM_Cardboard.SK_CharM_Cardboard"));
	if (SK_CARDBOARD.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SK_CARDBOARD.Object);
	}

	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	static ConstructorHelpers::FClassFinder<UAnimInstance> WARRIOR_ANIM(TEXT("/Game/Book/animations/WarriorAnimBluePrint.WarriorAnimBluePrint_C"));
	if (WARRIOR_ANIM.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(WARRIOR_ANIM.Class);
	}


	//SetControlMode(0);
	//SetControlMode(EControlMode::GTA);
	SetControlMode(EControlMode::DIABLO);

	//�ڿ������� ȸ�� ���� �Ӽ�
	ArmLengthSpeed = 3.0f;
	ArmRotationSpeed = 10.0f;

	// ���� ���� �Ӽ�
	// ������ �� �����°� �ƴ϶� �ѹ� ������ ���� �ϴ°� �̱� ������ Bind�� �ƴ� Action�̴�. 
	// ���� �𸮾󿡼� ������ Ű�� ���� ���ָ� �ڵ����� �������༭ �󸶳� ���� ������ ������ ���� ���� ������ �ָ� �ȴ�.
	GetCharacterMovement()->JumpZVelocity = 800.0f;

	//���������� �ƴ��� bool��
	IsAttacking = false;

	// ��Ƽ���� ��� ���� ��� ��������� ����
	MaxCombo = 4;
	AttackEndComboState();

	// �ݸ��� ĸ�� ������Ʈ�� �ش� �ݸ��� �������� ����ϵ��� ���� (�⺻���� Pawn)
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("ABCharacter"));

	// DrawDebug
	AttackRange = 200.0f;
	AttackRadius = 50.0f;

	// ĳ���� �������� �� (Actor ��� x)
	/*
	FName WeaponSocket("Hand_rSocket");
	if (GetMesh()->DoesSocketExist(WeaponSocket))
	{
		Weapon = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WEAPON"));
		static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_WEAPON(TEXT("SkeletalMesh'/Game/InfinityBladeWeapons/Weapons/Blade/Swords/Blade_BlackKnight/SK_Blade_BlackKnight.SK_Blade_BlackKnight'"));
		if (SK_WEAPON.Succeeded())
		{
			Weapon->SetSkeletalMesh(SK_WEAPON.Object);
		}
		Weapon->SetupAttachment(GetMesh(), WeaponSocket);
	}
	*/

	// UI �����߰� 
	HPBarWidget->SetRelativeLocation(FVector(0.0f, 0.0f, 180.0f));
	HPBarWidget->SetWidgetSpace(EWidgetSpace::Screen);
	static ConstructorHelpers::FClassFinder<UUserWidget> UI_HUD(TEXT("WidgetBlueprint'/Game/Book/UI/UI_HPBar.UI_HPBar_C'"));
	if (UI_HUD.Succeeded())
	{
		HPBarWidget->SetWidgetClass(UI_HUD.Class);
		HPBarWidget->SetDrawSize(FVector2D(150.0f, 50.0f));
	}

	// AI ��Ʈ�ѷ� 
	// AIControllerClass, AutoPossessAI�� APawn �ȿ� �ִ� AI ���� ����
	AIControllerClass = AABAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;


	// INI���� �о����
	auto DefaultSetting = GetDefault<UABCharacterSetting>();
	if (DefaultSetting->CharacterAssets.Num() > 0)
	{
		for (auto CharacterAsset : DefaultSetting->CharacterAssets)
		{
			ABLOG(Warning, TEXT("Character Asset : %s"), *CharacterAsset.ToString());
		}
	}

	// 14�� ���� ����ܰ� ������
	AssetIndex = 4;
	SetActorHiddenInGame(true);
	HPBarWidget->SetHiddenInGame(true);
	//bCanBeDamaged ������ private�� �Ǿ��ֱ� ������ SetCanBeDamaged�� ����ؾ� �Ѵ�.
	//bCanBeDamaged = false;
	SetCanBeDamaged(false);

	DeadTimer = 5.0f;
}

void AABCharacter::SetCharacterState(ECharacterState NewState)
{
	ABCHECK(CurrentState != NewState);
	CurrentState = NewState;

	switch (CurrentState)
	{
	case ECharacterState::LOADING:
	{
		if (bIsPlayer)
		{
			// �÷��̾� �Է��� ABPlayerController �޴°� �ߴ�
			DisableInput(ABPlayerController);

			// ������ ���� ������ UI ����
			ABPlayerController->GetHUDWidget()->BindCharacterStat(CharacterStat);

			auto ABPlayerState = Cast<AABPlayerState>(GetPlayerState());
			ABCHECK(nullptr != ABPlayerState);
			CharacterStat->SetNewLevel(ABPlayerState->GetCharacterLevel());
		}

		SetActorHiddenInGame(true);
		HPBarWidget->SetHiddenInGame(true);
		SetCanBeDamaged(false);
		break;
	}
	case ECharacterState::READY:
	{
		SetActorHiddenInGame(false);
		HPBarWidget->SetHiddenInGame(false);
		SetCanBeDamaged(true);

		CharacterStat->OnHPIsZero.AddLambda([this]()-> void {
			SetCharacterState(ECharacterState::DEAD);
			});

		auto CharacterWidget = Cast<UABCharacterWidget>(HPBarWidget->GetUserWidgetObject());
		ABCHECK(CharacterWidget != nullptr);
		CharacterWidget->BindCharacterStat(CharacterStat);

		if (bIsPlayer)
		{
			SetControlMode(EControlMode::DIABLO);
			GetCharacterMovement()->MaxWalkSpeed = 600.0f;
			// �÷��̾� �Է��� ABPlayerController �޾Ƶ��� DisableInput�� �ݴ�
			EnableInput(ABPlayerController);
		}
		else
		{
			SetControlMode(EControlMode::NPC);
			GetCharacterMovement()->MaxWalkSpeed = 400.0f;
			ABAIController->RunAI();
		}
		break;
	}
	case ECharacterState::DEAD:
	{
		SetActorHiddenInGame(false);
		GetMesh()->SetHiddenInGame(false);
		HPBarWidget->SetHiddenInGame(true);

		ABAnim->SetDeadAnim();
		SetCanBeDamaged(false);

		if (bIsPlayer)
		{
			// �÷��̾� �Է��� ABPlayerController �޴°� �ߴ�
			DisableInput(ABPlayerController);
		}
		else
		{
			ABAIController->StopAI();
		}
		GetWorld()->GetTimerManager().SetTimer(DeadTimerHandle, FTimerDelegate::CreateLambda([this]()->void {
			if (bIsPlayer)
			{
				ABPlayerController->RestartLevel();
			}
			else
			{
				Destroy();
			}
			}), DeadTimer, false);

		break;
	}
	}
}

ECharacterState AABCharacter::GetCharacterState() const
{
	return CurrentState;
}


// Called when the game starts or when spawned
void AABCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	// ���⼳�� (Actor ���)
	// WeaponSoket �̶� ������ hand_rSocket ���� (hand_rSocket �� ������ �� ��ġ�� �ǹ�)
	
	//FName WeaponSocket(TEXT("hand_rSocket"));

	/*
	auto CurWeapon = GetWorld()->SpawnActor<AABWeapon>(FVector::ZeroVector, FRotator::ZeroRotator);
	if (CurWeapon != nullptr)
	{
		CurWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponSocket);// �������� ������ġ �ǹ�
	}
	*/
	// 13�� 468p ĳ���� ���� ini �ε��Ѱ� �������� ��������
	
	// ���� �������Ʈ�� 4.21 �̻���ʹ� PostIniz ���� Begin���� �ؾ��� ( �׷��� ������ å �Ĺ� �ڵ��κ� ����) 
	// ���� HP ����
	auto CharacterWidget = Cast<UABCharacterWidget>(HPBarWidget->GetUserWidgetObject());
	if (nullptr != CharacterWidget)
	{
		CharacterWidget->BindCharacterStat(CharacterStat);
	}
	// ������Ʈ �ӽ� �𵨷� ���� ����ܰ� 14�� ���� 501p 
	/*
	if (!IsPlayerControlled())
	{
		auto DefaultSetting = GetDefault<UABCharacterSetting>();
		int32 RandIndex = FMath::RandRange(0, DefaultSetting->CharacterAssets.Num() - 1);
		CharacterAssetToLoad = DefaultSetting->CharacterAssets[RandIndex];

		auto ABGameInstance = Cast<UABGameInstance>(GetGameInstance());
		if (nullptr != ABGameInstance)
		{
			AssetStreamingHandle = ABGameInstance->StreamableManager.RequestAsyncLoad(CharacterAssetToLoad, 
				FStreamableDelegate::CreateUObject(this, &AABCharacter::OnAssetLoadCompleted));
		}
	}
	*/
	bIsPlayer = IsPlayerControlled();
	if (bIsPlayer)
	{
		ABPlayerController = Cast<AABPlayerController>(GetController());
		ABCHECK(nullptr != ABPlayerController);
	}
	else
	{
		ABAIController = Cast<AABAIController>(GetController());
		ABCHECK(nullptr != ABAIController);
	}

	auto DefaultSetting = GetDefault<UABCharacterSetting>();

	if (bIsPlayer)
	{
		AssetIndex = 4;
	}
	else
	{
		AssetIndex = FMath::RandRange(0, DefaultSetting->CharacterAssets.Num() - 1);
	}
	
	CharacterAssetToLoad = DefaultSetting->CharacterAssets[AssetIndex];
	auto ABGameInstance = Cast<UABGameInstance>(GetGameInstance());
	ABCHECK(nullptr != ABGameInstance);
	AssetStreamingHandle = ABGameInstance->StreamableManager.RequestAsyncLoad(CharacterAssetToLoad,
	FStreamableDelegate::CreateUObject(this, &AABCharacter::OnAssetLoadCompleted));
	SetCharacterState(ECharacterState::LOADING);
}

// Called every frame
void AABCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	SpringArm->TargetArmLength = FMath::FInterpTo(SpringArm->TargetArmLength, ArmLengthTo, DeltaTime, ArmLengthSpeed);
	switch (CurrentControlMode)
	{
	case EControlMode::DIABLO:
		
		SpringArm->SetRelativeRotation(FMath::RInterpTo(SpringArm->GetRelativeRotation(), ArmRotationTo, DeltaTime, ArmRotationSpeed));
		// DirectionToMove�� ���� �����Ѱ�
		if (DirectionToMove.SizeSquared() > 0.0f)
		{
			// MakeFromX���� ���� ȸ������� Rotator�� ����. �� rotator�� ���� ��ǥ�迡�� 
			// �Է� �Ķ���� ���͸� X������ �ϴ� ��ǥ��� �����ϱ� ���� �ʿ��� ȸ�����̴�.
			// FRotationMatrix::MakeFromX(DirectionToMove).Rotator() �� �ϰԵǸ�  
			// ��ǥ���� x=0 y=1 z=0�� �Ǿ� y�� �ش�Ǵ� Yaw���� Rotation���� ȯ���Ͽ� Yaw 90.0f�� �����س���.
			// ���� x=1 y=1 z=0 �̸� yaw���� 45.0f ���ȴ�.
			
			GetController()->SetControlRotation(FRotationMatrix::MakeFromX(DirectionToMove).Rotator());
			AddMovementInput(DirectionToMove);
		}
		break;
	}

}


// Called to bind functionality to input
void AABCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// ������Ʈ -> �Է� ���� Ű ��� �ؾ���
	PlayerInputComponent->BindAxis(TEXT("UpDown"), this, &AABCharacter::UpDown);
	PlayerInputComponent->BindAxis(TEXT("LeftRight"), this, &AABCharacter::LeftRight);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &AABCharacter::LookUp);
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &AABCharacter::Turn);
	

	PlayerInputComponent->BindAction(TEXT("ViewChange"), EInputEvent::IE_Pressed, this, &AABCharacter::ViewChange);
	//������ �� �����°� �ƴ϶� �ѹ� ������ ���� �ϴ°� �̱� ������ Axis�� �ƴ� Action�̴�.
	PlayerInputComponent->BindAction(TEXT("Jump"),EInputEvent::IE_Pressed ,this, &AABCharacter::Jump);
	//���ݸ��
	PlayerInputComponent->BindAction(TEXT("Attack"), EInputEvent::IE_Pressed, this, &AABCharacter::Attack);
}

//void AABCharacter::SetControlMode(int32 ControlMode)
//{
//	if (ControlMode == 0)
//	{
//		SpringArm->TargetArmLength = 450.0f;
//		SpringArm->SetRelativeRotation(FRotator::ZeroRotator);
//		SpringArm->bUsePawnControlRotation = true;
//		SpringArm->bInheritPitch = true;
//		SpringArm->bInheritRoll = true;
//		SpringArm->bInheritYaw = true;
//		SpringArm->bDoCollisionTest = true;
//		bUseControllerRotationYaw = false;
//
//		GetCharacterMovement()->bOrientRotationToMovement = true;
//		GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.0f, 0.0f);
//
//	}
//}
void AABCharacter::SetControlMode(EControlMode ControlMode)
{
	CurrentControlMode = ControlMode;

	switch (CurrentControlMode)
	{
	case EControlMode::GTA:
		//SpringArm->TargetArmLength = 450.0f;
		////�ʱ� ȸ�� ���� ����
		//SpringArm->SetRelativeRotation(FRotator::ZeroRotator);
		ArmLengthTo = 450.0f;

		// ���������� ȸ�� ���� ��Ʈ�� ȸ�� ���� �����ϰ� �����ش�.
		// springarm�� root������Ʈ�� ȸ���� �� ���� ȸ���ϴ°� ����
		SpringArm->bUsePawnControlRotation = true;
		SpringArm->bInheritPitch = true;
		SpringArm->bInheritRoll = true;
		SpringArm->bInheritYaw = true;
		// bDoCollisionTest �� ���� ī�޶�� ĳ���� ���̿� ��ֹ��� �־ �� ����� �۵����� �ʴ´�.
		SpringArm->bDoCollisionTest = true;

		// ��Ʈ�ѷ��� ȸ�� �������� ĳ���͸� ȸ���Ѵ�.
		// 4.26 ���� bOrientRotationToMovement �� ���� ��ü��.
		bUseControllerRotationYaw = false; 

		// ĳ���Ͱ� �����̴� �������� ĳ���͸� �ڵ����� ȸ������ �ش�. ��
		GetCharacterMovement()->bOrientRotationToMovement = true;

		GetCharacterMovement()->bUseControllerDesiredRotation = false;

		// ȸ���� �ε巴�� ����� �ֱ� ���� RotationRate �� �����Ѵ�.
		GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.0f, 0.0f);
		break;
	case EControlMode::DIABLO:
		
		//SpringArm->TargetArmLength = 800.0f;
		//SpringArm->SetRelativeRotation(FRotator(-45.0f, 0.0f, 0.0f));
		ArmLengthTo = 800.0f;
		ArmRotationTo = FRotator(-45.0f, 0.0f, 0.0f);

		SpringArm->bUsePawnControlRotation = false;
		// springarm�� root������Ʈ�� ȸ���� �� ���� ȸ���ϴ°� ����
		SpringArm->bInheritPitch = false;
		SpringArm->bInheritRoll = false;
		SpringArm->bInheritYaw = false;
		// bDoCollisionTest �� ���� ī�޶�� ĳ���� ���̿� ��ֹ��� �־ �� ����� �۵����� �ʴ´�.
		SpringArm->bDoCollisionTest = false;
		//ĳ������ ȸ�� �����ϰ� �ٷ� ȸ���� �ϰ� �ȴ�.	
		//bUseControllerRotationYaw = true;
		bUseControllerRotationYaw = false;
		GetCharacterMovement()->bOrientRotationToMovement = false;
		// bUseControllerDesiredRotation�� bUseControllerRotationYaw ��ſ� ����ϸ� ĳ���Ͱ� �ε巴�� ȸ���Ѵ�.
		GetCharacterMovement()->bUseControllerDesiredRotation = true;
		// ȸ���� �ε巴�� ����� �ֱ� ���� RotationRate �� �����Ѵ�.
		GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.0f, 0.0f);
		break;
	// NPC �ϰ�� ���
	case EControlMode::NPC:
		bUseControllerRotationYaw = false;
		GetCharacterMovement()->bUseControllerDesiredRotation = false;
		GetCharacterMovement()->bOrientRotationToMovement = true;
		GetCharacterMovement()->RotationRate = FRotator(0.0f, 480.0f, 0.0f);
		break;
	}
}
// ������ ����� ���⺯��
bool AABCharacter::CanSetWeapon()
{
	return (nullptr == CurrentWeapon);
}

void AABCharacter::SetWeapon(AABWeapon* NewWeapon)
{
	ABCHECK(nullptr != NewWeapon && nullptr == CurrentWeapon);
	FName WeaponSocket(TEXT("hand_rSocket"));
	if (nullptr != NewWeapon)
	{
		NewWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponSocket);
		NewWeapon->SetOwner(this);
		CurrentWeapon = NewWeapon;
	}
}
//
void AABCharacter::UpDown(float NewAxisValue)
{
	//GetActorForwardVector()�� GetActorRightVector()�� ����ϸ� ���Ͱ� ���ϴ� ������ �������� �̵��ϴ� ���� ����
	//AddMovementInput(GetActorForwardVector(), NewAxisValue);
	//AddMovementInput(FRotationMatrix(GetControlRotation()).GetUnitAxis(EAxis::X), NewAxisValue);
	switch (CurrentControlMode)
	{
	case EControlMode::GTA:
		AddMovementInput(FRotationMatrix(FRotator(0.0f, GetControlRotation().Yaw, 0.0f)).GetUnitAxis(EAxis::X), NewAxisValue);
		break;
	case EControlMode::DIABLO:
		// DIABLO ����ϰ�� Tick ���� AddMovementInput�� ����Ѵ�.
		DirectionToMove.X = NewAxisValue;
		break;
	}
}

void AABCharacter::LeftRight(float NewAxisValue)
{
	//AddMovementInput(GetActorRightVector(), NewAxisValue);
	//AddMovementInput(FRotationMatrix(GetControlRotation()).GetUnitAxis(EAxis::Y), NewAxisValue);
	switch (CurrentControlMode)
	{
	case EControlMode::GTA:
		AddMovementInput(FRotationMatrix(FRotator(0.0f,GetControlRotation().Yaw,0.0f)).GetUnitAxis(EAxis::Y), NewAxisValue);
		break;
	case EControlMode::DIABLO:
		// DIABLO ����ϰ�� Tick ���� AddMovementInput�� ����Ѵ�.
		DirectionToMove.Y = NewAxisValue;
		break;
	}
}
void AABCharacter::LookUp(float NewAxisValue)
{
	//AddControllerPitchInput(NewAxisValue);
	switch (CurrentControlMode)
	{
	case EControlMode::GTA:
		AddControllerPitchInput(NewAxisValue);
		break;
	}
}
void AABCharacter::Turn(float NewAxisValue)
{
	//AddControllerYawInput(NewAxisValue);
	switch (CurrentControlMode)
	{
	case EControlMode::GTA:
		AddControllerYawInput(NewAxisValue);
		break;
	}
}
void AABCharacter::ViewChange()
{
	switch (CurrentControlMode)
	{
	case EControlMode::GTA:
		GetController()->SetControlRotation(GetActorRotation());
		SetControlMode(EControlMode::DIABLO);
		break;
	case EControlMode::DIABLO:
		GetController()->SetControlRotation(SpringArm->GetRelativeRotation());
		SetControlMode(EControlMode::GTA);
		break;
	}
}

void AABCharacter::Attack()
{
	/*

	// ���� �������̶�� ����Ŭ�� ����
	if (IsAttacking) return;

	//auto AnimInstance = Cast<UABAnimInstance>(GetMesh()->GetAnimInstance());
	//if (nullptr == AnimInstance)
	if (nullptr == ABAnim)
	{
		return;
	}

	// ��Ÿ�� ����
	//AnimInstance->PlayAttackMontage();
	// �̸� Ŭ�������� �Ͽ� Post���� �޽��� ��� ABAnim�� ��������� ����ϰ� �ۼ�
	ABAnim->PlayAttackMontage();

	IsAttacking = true;
	*/
	//��Ƽ���� ��� ����
	
	if (IsAttacking)
	{
		ABCHECK(FMath::IsWithinInclusive<int32>(CurrentCombo, 1, MaxCombo));
		if (CanNextCombo)
		{
			IsComboInputOn = true;
		}
	}
	else
	{
		ABCHECK(CurrentCombo == 0);
		AttackStartComboState();
		ABAnim->PlayAttackMontage();
		ABAnim->JumpToAttackMontageSection(CurrentCombo);
		IsAttacking = true;
	}
	ABLOG_S(Warning);
}

// OnMontageEnded�� ���ε��� �Լ�
void AABCharacter::OnAttackMentageEnded(UAnimMontage* Montage, bool BInterrupted)
{
	ABCHECK(IsAttacking);
	//��Ƽ���� ��� ����
	ABCHECK(CurrentCombo > 0);
	IsAttacking = false;
	AttackEndComboState();

	// ���� ���� ��������Ʈ ȣ��
	OnAttackEnd.Broadcast();
}

// ��������Ʈ �
// PostInitializeComponents �� BeginPlay ������ ����ȴ�.
void AABCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	// GetMesh() �� ĳ���� �ϳ��� �ϳ��ۿ� ���� ������ �ش�ĳ���� ��������� GetMesh()�� �ϸ� ���� ���� ������ �ȴ�.
	//auto AnimInstance = Cast<UABAnimInstance>(GetMesh()->GetAnimInstance());
	// ABAnim Ŭ���� ����ȭ �Ѱ� ĳ������ �޽��� ����

	ABAnim = Cast<UABAnimInstance>(GetMesh()->GetAnimInstance());
	//ABCHECK(nullptr != AnimInstance);
	ABCHECK(nullptr != ABAnim);

	// ��������Ʈ ����
	// UABAnumInstance �ȿ� OnMontageEnded�� ����.
	// �ش� ��Ÿ�ְ� ������ OnAttackMentageEnded ȣ��
	// Attack() ���� GetMesh()�� �Ͽ� ��Ÿ�ָ� �۵��Ͽ��� ������ �ش� ��Ÿ�ְ� �������� �ȳ������� �� �� �ִ�.
	//AnimInstance->OnMontageEnded.AddDynamic(this, &AABCharacter::OnAttackMentageEnded);
	ABAnim->OnMontageEnded.AddDynamic(this, &AABCharacter::OnAttackMentageEnded);

	// ��Ƽ���� ��� ����
	ABAnim->OnNextAttackCheck.AddLambda([this]() -> void {
		ABLOG(Warning, TEXT("OnNextAttackCheck"));
		CanNextCombo = false;
		if (IsComboInputOn)
		{
			AttackStartComboState();
			ABAnim->JumpToAttackMontageSection(CurrentCombo);
		}
	});
	// �浹���� (�ݸ���)
	ABAnim->OnAttackHitCheck.AddUObject(this, &AABCharacter::AttackCheck);

	//  ����� ���� �� HP <= 0 �̸� �״¸�� ��������Ʈ �����а� ����
	CharacterStat->OnHPIsZero.AddLambda([this]() -> void {
		ABLOG(Warning, TEXT("OnHPIsZero ��������Ʈ �۵�(ABCharacter.cpp)"));
		ABAnim->SetDeadAnim();
		SetActorEnableCollision(false);
		});

	// ���� UI�߰�
	// 4.21 �̻���ʹ� PostInitialize ���ƴ� Begin���� �ؾ���
	/*
	auto CharacterWidget = Cast<UABCharacterWidget>(HPBarWidget->GetUserWidgetObject());
	if (nullptr != CharacterWidget)
	{
		CharacterWidget->BindCharacterStat(CharacterStat);
	}
	*/
}

void AABCharacter::AttackStartComboState()
{
	CanNextCombo = true;
	IsComboInputOn = false;
	ABCHECK(FMath::IsWithinInclusive<int32>(CurrentCombo, 0, MaxCombo - 1));
	CurrentCombo = FMath::Clamp<int32>(CurrentCombo + 1, 1, MaxCombo);
}

void AABCharacter::AttackEndComboState()
{
    IsComboInputOn = false;
    CanNextCombo = false;
    CurrentCombo = 0;
}
// �浹���� (�ݸ���)
void AABCharacter::AttackCheck()
{
	FHitResult HitResult;
	FCollisionQueryParams Params(NAME_None, false, this);
	bool bResult = GetWorld()->SweepSingleByChannel(
		HitResult,
		GetActorLocation(),
		GetActorLocation() + GetActorForwardVector() * 200.0f,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel2,
		FCollisionShape::MakeSphere(50.0f),
		Params
	);

	// DrawDebug
#if ENABLE_DRAW_DEBUG
	FVector TraceVec = GetActorForwardVector() * AttackRange;
	FVector Center = GetActorLocation() + TraceVec * 0.5f;
	float HalfHeight = AttackRange * 0.5f + AttackRadius;
	FQuat CapsuleRot = FRotationMatrix::MakeFromZ(TraceVec).ToQuat();
	FColor DrawColor = bResult ? FColor::Green : FColor::Red;
	float DebugLifeTime = 5.0f;
	
	DrawDebugCapsule(GetWorld(),
		Center,
		HalfHeight,
		AttackRadius,
		CapsuleRot,
		DrawColor,
		false,
		DebugLifeTime
	);

#endif

	//

	if (bResult)
	{
		if (HitResult.Actor.IsValid())
		{
			ABLOG(Warning, TEXT("Hit Actor Name : %s"), *HitResult.Actor->GetName());
		}
		
		FDamageEvent DamageEvent;
		//HitResult.Actor->TakeDamage(50.0f, DamageEvent, GetController(), this);
		HitResult.Actor->TakeDamage(CharacterStat->GetAttack(), DamageEvent, GetController(), this);
	}


}

// 13�� 468p ĳ���� ���� ini �ε��Ѱ� �������� ��������
void AABCharacter::OnAssetLoadCompleted()
{
	USkeletalMesh* AssetLoaded = Cast<USkeletalMesh>(AssetStreamingHandle->GetLoadedAsset());
	/*
	if (nullptr != AssetLoaded)
	{
		GetMesh()->SetSkeletalMesh(AssetLoaded);
	}
	*/
	// 14�� 508p
	AssetStreamingHandle.Reset();
	ABCHECK(nullptr != AssetLoaded);
	GetMesh()->SetSkeletalMesh(AssetLoaded);

	SetCharacterState(ECharacterState::READY);
}

float AABCharacter::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
	float FinalDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	ABLOG(Warning, TEXT("Actor : %s took Damage : %f"), *GetName(), FinalDamage);
	// �̺κ� ������ ��������Ʈ���� ����
	/*
	if (FinalDamage > 0.0f)
	{
		ABAnim->SetDeadAnim();
		SetActorEnableCollision(false);
	}
	*/
	// ����� ���� �� HP <= 0 �̸� �״¸�� ��������Ʈ �۵�
	CharacterStat->SetDamage(FinalDamage);

	return FinalDamage;
}

void AABCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	
	if (IsPlayerControlled())
	{
		SetControlMode(EControlMode::DIABLO);
		GetCharacterMovement()->MaxWalkSpeed = 600.0f;
	}
	else
	{
		SetControlMode(EControlMode::NPC);
		GetCharacterMovement()->MaxWalkSpeed = 300.0f;
	}
}
