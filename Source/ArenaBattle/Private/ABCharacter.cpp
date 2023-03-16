// Fill out your copyright notice in the Description page of Project Settings.


#include "ABCharacter.h"

#include "ABAnimInstance.h"
// DrawDebug
#include "DrawDebugHelpers.h"

// 무기 장착을 위한 해더파일 begin 에서 설정
#include "ABWeapon.h"

// 데이터 파일
#include "ABCharacterStatComponent.h"

// UI 위젯
#include "Components/WidgetComponent.h"
#include "ABCharacterWidget.h"

// AI 컨트롤러 
#include "ABAIController.h"

// INI파일 읽어오기
#include "ABCharacterSetting.h"

// 13장 a
#include "ABGameInstance.h"

// 14장
#include "ABPlayerController.h"
// 플레이어 데이터(정보)와 UI연동하기 위한 해더 ex) 게임점수 , 이름(닉네임), 레벨
#include "ABPlayerState.h"
#include "ABHUDWidget.h"
// 15장 스코어 기반 AI레벨 설정
#include "ABGameMode.h"

// 캐릭터 오브젝트 -> 액터 -> 폰 -> 캐릭터 순으로 상속함  오브젝트가 Root부모
// 캐릭터는 APawn에서 좀 더 복잡한 애니메이션을 위해 파생된 클래스라고한다. 
// 예를들어 점프하고 수영하고, 하늘을 날아다니는 에니메이션들과 그 에니메이션들을 블렌딩하는 작업 등등 복잡한 에니매이션을 처리할 수 있도록 고안
// 캐릭터에서 오브젝트 설정하고 InputComponent에서 Input 관련 설정
// Sets default values
AABCharacter::AABCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SPRINGARM"));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CAMERA"));
	// UI 위젯추가
	HPBarWidget = CreateDefaultSubobject<UWidgetComponent>("HPBARWIDGET");
	
	SpringArm->SetupAttachment(GetCapsuleComponent());
	Camera->SetupAttachment(SpringArm);
	// UI 위젯추가 
	HPBarWidget->SetupAttachment(GetMesh());

	//Pawn에서는 그냥 Mesh->를 쓰지만 여기서는 GetMesh()-> 사용함
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -88.0f), FRotator(0.0f, -90.0f, 0.0f));
	SpringArm->TargetArmLength = 400.0f;
	SpringArm->SetRelativeRotation(FRotator(-15.0f, 0.0f, 0.0f));

	// 11장 게임 데이터 테이블( 각종 스탯데이터 ) 설정
	// cpp 파일에서 include 추가
	CharacterStat = CreateDefaultSubobject<UABCharacterStatComponent>(TEXT("CHARACTERSTAT"));

	//폰 등륵 할 때는 Object를 사용
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

	//자연스러운 회전 관련 속성
	ArmLengthSpeed = 3.0f;
	ArmRotationSpeed = 10.0f;

	// 점프 관련 속성
	// 점프는 꾹 누르는게 아니라 한번 눌렀다 땠다 하는거 이기 때문에 Bind가 아닌 Action이다. 
	// 또한 언리얼에서 점프를 키만 연결 해주면 자동으로 구성해줘서 얼마나 높이 점프할 것인지 점프 값만 설정해 주면 된다.
	GetCharacterMovement()->JumpZVelocity = 800.0f;

	//공격중인지 아닌지 bool값
	IsAttacking = false;

	// 노티파이 모션 공격 모션 끊어놓은거 연결
	MaxCombo = 4;
	AttackEndComboState();

	// 콜리전 캡슐 컴포넌트가 해당 콜리전 프리셋을 사용하도록 지정 (기본값은 Pawn)
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("ABCharacter"));

	// DrawDebug
	//562p 무기 속성 무기있으면 150 없으면 80 만들기 위해 80변경
	AttackRange = 80.0f;
	AttackRadius = 50.0f;

	// 캐릭터 무기장착 모델 (Actor 사용 x)
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

	// UI 위젯추가 
	// 위젯의 위치 설정
	// HPBarWidget 컴포넌트는 Character 의 Mesh 에 부착되어 있으므로  Character의 좌표에서 180.0f 위치에 부착 되어있는걸로 추측
	HPBarWidget->SetRelativeLocation(FVector(0.0f, 0.0f, 180.0f));
	// 위젯의 설정
	/*
	Screen: 2D 위젯은 화면상에 고정됩니다. 카메라 이동에 관계없이 항상 일정한 위치에 머무릅니다.
	World: 2D 위젯은 3D 공간에 배치됩니다. 카메라 이동에 따라 이동합니다.
	Local: 2D 위젯은 부모 오브젝트에 상대적인 위치에 배치됩니다.
	**/
	HPBarWidget->SetWidgetSpace(EWidgetSpace::Screen);
	static ConstructorHelpers::FClassFinder<UUserWidget> UI_HUD(TEXT("WidgetBlueprint'/Game/Book/UI/UI_HPBar.UI_HPBar_C'"));
	if (UI_HUD.Succeeded())
	{
		HPBarWidget->SetWidgetClass(UI_HUD.Class);
		// 위젯의 크기설정
		HPBarWidget->SetDrawSize(FVector2D(150.0f, 50.0f));
	}

	// AI 컨트롤러 
	// AIControllerClass, AutoPossessAI는 APawn 안에 있는 AI 관련 변수
	AIControllerClass = AABAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;


	// INI파일 읽어오기
	auto DefaultSetting = GetDefault<UABCharacterSetting>();
	if (DefaultSetting->CharacterAssets.Num() > 0)
	{
		for (auto CharacterAsset : DefaultSetting->CharacterAssets)
		{
			ABLOG(Warning, TEXT("Character Asset : %s"), *CharacterAsset.ToString());
		}
	}

	// 14장 게임 진행단계 나누기
	AssetIndex = 4;
	SetActorHiddenInGame(true);
	HPBarWidget->SetHiddenInGame(true);
	//bCanBeDamaged 변수는 private로 되어있기 때문에 SetCanBeDamaged를 사용해야 한다.
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
			// 플레이어 입력을 ABPlayerController 받는걸 중단
			DisableInput(ABPlayerController);

			// 데이터 엑터 저정후 UI 연동
			ABPlayerController->GetHUDWidget()->BindCharacterStat(CharacterStat);

			auto ABPlayerState = Cast<AABPlayerState>(GetPlayerState());
			ABCHECK(nullptr != ABPlayerState);
			CharacterStat->SetNewLevel(ABPlayerState->GetCharacterLevel());
		}
		// 15장 스코어 기반 AI레벨 설정 코드추가
		else
		{
			// GetAuthGameMode : 현재 실행 중인 서버에서 사용 중인 게임 모드 클래스를 반환하는 함수
			auto ABGameMode = Cast<AABGameMode>(GetWorld()->GetAuthGameMode());
			ABCHECK(nullptr != ABGameMode);
			int32 TargetLevel = FMath::CeilToInt(((float)ABGameMode->GetScore() * 0.8f));
			int32 FinalLevel = FMath::Clamp<int32>(TargetLevel, 1, 20);
			ABLOG(Warning, TEXT("New NPC Level : %d"), FinalLevel);
			CharacterStat->SetNewLevel(FinalLevel);
		}
		//
		
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
		// 로그 출력 2번 되기 떄문에 주석처리 321번쨰줄 Begin에서 이미 BInd 해주었기 떄문에
		// 굳이 여기서 해줄 필요가 없다.
		//CharacterWidget->BindCharacterStat(CharacterStat);

		if (bIsPlayer)
		{
			SetControlMode(EControlMode::DIABLO);
			GetCharacterMovement()->MaxWalkSpeed = 600.0f;
			// 플레이어 입력을 ABPlayerController 받아들임 DisableInput의 반대
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
			// 플레이어 입력을 ABPlayerController 받는걸 중단
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

int32 AABCharacter::GetExp() const
{
	return CharacterStat->GetDropExp();
}

float AABCharacter::GetFinalAttackRange() const
{
	return (nullptr != CurrentWeapon) ? CurrentWeapon->GetAttackRange() : AttackRange;
}

float AABCharacter::GetFinalAttackDamage() const
{
	float AttackDamage = (nullptr != CurrentWeapon) ?
		(CharacterStat->GetAttack() + CurrentWeapon->GetAttackDamage()) : CharacterStat->GetAttack();
	float AttackModifier = (nullptr != CurrentWeapon) ?
		CurrentWeapon->GetAttackModifier() : 1.0f;
	return AttackDamage * AttackModifier;
}


// Called when the game starts or when spawned
void AABCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	// 무기설정 (Actor 사용)
	// WeaponSoket 이란 변수에 hand_rSocket 저장 (hand_rSocket 은 오른쪽 손 위치를 의미)
	
	//FName WeaponSocket(TEXT("hand_rSocket"));

	/*
	auto CurWeapon = GetWorld()->SpawnActor<AABWeapon>(FVector::ZeroVector, FRotator::ZeroRotator);
	if (CurWeapon != nullptr)
	{
		CurWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponSocket);// 마지막은 소켓위치 의미
	}
	*/
	// 13장 468p 캐릭터 외형 ini 로드한거 랜덤으로 가져오기
	
	// 위젯 블루프린트는 4.21 이상부터는 PostIniz 말고 Begin에서 해야함 ( 그렇기 떄문에 책 후반 코딩부분 조심) 
	// 위젯 HP 연동
	auto CharacterWidget = Cast<UABCharacterWidget>(HPBarWidget->GetUserWidgetObject());
	if (nullptr != CharacterWidget)
	{
		CharacterWidget->BindCharacterStat(CharacterStat);
	}
	// 스테이트 머신 모델로 게임 진행단계 14장 구분 501p 
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

	// 현재 액터가 플레이어에 의해 제어되는지 여부를 확인
	// 플레이어가 조종하면 true 아니면 false
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
		//AssetIndex = 4;
		// 595p 선택한 캐릭터 Index 가져오기
		// 코드에서는 GetPlayerState() 말고 PlayerState를 가져오는걸로 되어있는데
		// 오류발생함
		auto ABPlayerState = Cast<AABPlayerState>(GetPlayerState());
		ABCHECK(nullptr != ABPlayerState);
		AssetIndex = ABPlayerState->GetCharacterIndex();
		ABLOG(Warning, TEXT("인덱스 : %d"), ABPlayerState->GetCharacterIndex());
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
		// DirectionToMove는 변수 선언한것
		if (DirectionToMove.SizeSquared() > 0.0f)
		{
			// MakeFromX에서 만든 회전행렬의 Rotator를 리턴. 이 rotator는 월드 좌표계에서 
			// 입력 파라미터 벡터를 X축으로 하는 좌표계로 이전하기 위해 필요한 회전각이다.
			// FRotationMatrix::MakeFromX(DirectionToMove).Rotator() 을 하게되면  
			// 좌표선상 x=0 y=1 z=0이 되어 y에 해당되는 Yaw값을 Rotation으로 환산하여 Yaw 90.0f를 도출해낸다.
			// 만약 x=1 y=1 z=0 이면 yaw값은 45.0f 가된다.
			
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

	// 프로젝트 -> 입력 에서 키 등륵을 해야함
	PlayerInputComponent->BindAxis(TEXT("UpDown"), this, &AABCharacter::UpDown);
	PlayerInputComponent->BindAxis(TEXT("LeftRight"), this, &AABCharacter::LeftRight);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &AABCharacter::LookUp);
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &AABCharacter::Turn);
	

	PlayerInputComponent->BindAction(TEXT("ViewChange"), EInputEvent::IE_Pressed, this, &AABCharacter::ViewChange);
	//점프는 꾹 누르는게 아니라 한번 눌렀다 땠다 하는거 이기 때문에 Axis가 아닌 Action이다.
	PlayerInputComponent->BindAction(TEXT("Jump"),EInputEvent::IE_Pressed ,this, &AABCharacter::Jump);
	//공격모션
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
		////초기 회전 각도 설정
		//SpringArm->SetRelativeRotation(FRotator::ZeroRotator);
		ArmLengthTo = 450.0f;

		// 스프링암의 회전 값을 컨트롤 회전 값과 동일하게 맞춰준다.
		// springarm이 root오브젝트가 회전할 때 같이 회전하는거 방지
		SpringArm->bUsePawnControlRotation = true;
		SpringArm->bInheritPitch = true;
		SpringArm->bInheritRoll = true;
		SpringArm->bInheritYaw = true;
		// bDoCollisionTest 를 끄면 카메라와 캐릭터 사이에 장애물이 있어도 줌 기능이 작동하지 않는다.
		SpringArm->bDoCollisionTest = true;

		// 컨트롤러의 회전 방향으로 캐릭터를 회전한다.
		// 4.26 부터 bOrientRotationToMovement 에 의해 대체됨.
		bUseControllerRotationYaw = false; 

		// 캐릭터가 움직이는 방향으로 캐릭터를 자동으로 회전시켜 준다. ★
		GetCharacterMovement()->bOrientRotationToMovement = true;

		GetCharacterMovement()->bUseControllerDesiredRotation = false;

		// 회전을 부드럽게 만들어 주기 위해 RotationRate 를 조정한다.
		GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.0f, 0.0f);
		break;
	case EControlMode::DIABLO:
		
		//SpringArm->TargetArmLength = 800.0f;
		//SpringArm->SetRelativeRotation(FRotator(-45.0f, 0.0f, 0.0f));
		ArmLengthTo = 800.0f;
		ArmRotationTo = FRotator(-45.0f, 0.0f, 0.0f);

		SpringArm->bUsePawnControlRotation = false;
		// springarm이 root오브젝트가 회전할 때 같이 회전하는거 방지
		SpringArm->bInheritPitch = false;
		SpringArm->bInheritRoll = false;
		SpringArm->bInheritYaw = false;
		// bDoCollisionTest 를 끄면 카메라와 캐릭터 사이에 장애물이 있어도 줌 기능이 작동하지 않는다.
		SpringArm->bDoCollisionTest = false;
		//캐릭터의 회전 딱딱하게 바로 회전을 하게 된다.	
		//bUseControllerRotationYaw = true;
		bUseControllerRotationYaw = false;
		GetCharacterMovement()->bOrientRotationToMovement = false;
		// bUseControllerDesiredRotation를 bUseControllerRotationYaw 대신에 사용하면 캐릭터가 부드럽기 회전한다.
		GetCharacterMovement()->bUseControllerDesiredRotation = true;
		// 회전을 부드럽게 만들어 주기 위해 RotationRate 를 조정한다.
		GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.0f, 0.0f);
		break;
	// NPC 일경우 모드
	case EControlMode::NPC:
		bUseControllerRotationYaw = false;
		GetCharacterMovement()->bUseControllerDesiredRotation = false;
		GetCharacterMovement()->bOrientRotationToMovement = true;
		GetCharacterMovement()->RotationRate = FRotator(0.0f, 480.0f, 0.0f);
		break;
	}
}
// 아이템 습득시 무기변경
bool AABCharacter::CanSetWeapon()
{
	//562p 무기 속성 에서 변경
	//return (nullptr == CurrentWeapon);
	return true;
}

void AABCharacter::SetWeapon(AABWeapon* NewWeapon)
{
	//562p 무기 속성 코드 추가 및 변경
	//ABCHECK(nullptr != NewWeapon && nullptr == CurrentWeapon);
	
	ABCHECK(nullptr != NewWeapon);
	if (nullptr != CurrentWeapon)
	{
		// DetachFromActor : 함수는 현재 액터를 다른 액터로부터 분리(detach)합니다.
		// 부모 액터에서 하위 계층에 있는 액터를 제거하고 싶을 때 유용
		CurrentWeapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		CurrentWeapon->Destroy();
		CurrentWeapon = nullptr;
	}


	//
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
	//GetActorForwardVector()와 GetActorRightVector()를 사용하면 액터가 향하는 방향을 기준으로 이동하는 것이 가능
	//AddMovementInput(GetActorForwardVector(), NewAxisValue);
	//AddMovementInput(FRotationMatrix(GetControlRotation()).GetUnitAxis(EAxis::X), NewAxisValue);
	switch (CurrentControlMode)
	{
	case EControlMode::GTA:
		AddMovementInput(FRotationMatrix(FRotator(0.0f, GetControlRotation().Yaw, 0.0f)).GetUnitAxis(EAxis::X), NewAxisValue);
		break;
	case EControlMode::DIABLO:
		// DIABLO 모드일경우 Tick 에서 AddMovementInput을 담당한다.
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
		// DIABLO 모드일경우 Tick 에서 AddMovementInput을 담당한다.
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

	// 만약 공격중이라면 연속클릭 방지
	if (IsAttacking) return;

	//auto AnimInstance = Cast<UABAnimInstance>(GetMesh()->GetAnimInstance());
	//if (nullptr == AnimInstance)
	if (nullptr == ABAnim)
	{
		return;
	}

	// 몽타주 실행
	//AnimInstance->PlayAttackMontage();
	// 미리 클래스변수 하여 Post에서 메쉬를 등륵한 ABAnim을 사용함으로 깔끔하게 작성
	ABAnim->PlayAttackMontage();

	IsAttacking = true;
	*/
	//노티파이 모션 연결
	
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

// OnMontageEnded에 바인딩할 함수
void AABCharacter::OnAttackMentageEnded(UAnimMontage* Montage, bool BInterrupted)
{
	ABCHECK(IsAttacking);
	//노티파이 모션 연결
	ABCHECK(CurrentCombo > 0);
	IsAttacking = false;
	AttackEndComboState();

	// 공격 종료 델리게이트 호출
	OnAttackEnd.Broadcast();
}

// 델리게이트 등륵
// PostInitializeComponents 는 BeginPlay 이전에 실행된다.
void AABCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	// GetMesh() 는 캐릭터 하나당 하나밖에 없기 떄문에 해당캐릭터 어느곳에서 GetMesh()를 하면 같은 값을 가지게 된다.
	//auto AnimInstance = Cast<UABAnimInstance>(GetMesh()->GetAnimInstance());
	// ABAnim 클래스 변수화 한걸 캐릭터의 메쉬를 저장

	ABAnim = Cast<UABAnimInstance>(GetMesh()->GetAnimInstance());
	//ABCHECK(nullptr != AnimInstance);
	ABCHECK(nullptr != ABAnim);

	// 델리게이트 설정
	// UABAnumInstance 안에 OnMontageEnded가 있음.
	// 해당 몽타주가 끝날시 OnAttackMentageEnded 호출
	// Attack() 에서 GetMesh()를 하여 몽타주를 작동하였기 떄문에 해당 몽타주가 끝났는지 안끝났는지 알 수 있다.
	//AnimInstance->OnMontageEnded.AddDynamic(this, &AABCharacter::OnAttackMentageEnded);
	ABAnim->OnMontageEnded.AddDynamic(this, &AABCharacter::OnAttackMentageEnded);

	// 노티파이 모션 연결
	ABAnim->OnNextAttackCheck.AddLambda([this]() -> void {
		ABLOG(Warning, TEXT("OnNextAttackCheck"));
		CanNextCombo = false;
		if (IsComboInputOn)
		{
			AttackStartComboState();
			ABAnim->JumpToAttackMontageSection(CurrentCombo);
		}
	});
	// 충돌설정 (콜리전)
	ABAnim->OnAttackHitCheck.AddUObject(this, &AABCharacter::AttackCheck);

	//  대미지 전달 후 HP <= 0 이면 죽는모션 델리게이트 만들어둔거 설정
	CharacterStat->OnHPIsZero.AddLambda([this]() -> void {
		ABLOG(Warning, TEXT("OnHPIsZero 델리게이트 Action(ABCharacter.cpp)"));
		ABAnim->SetDeadAnim();
		SetActorEnableCollision(false);
		});

	// 위젯 UI추가
	// 4.21 이상부터는 PostInitialize 가아닌 Begin에서 해야함
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
// 충돌설정 (콜리전)
void AABCharacter::AttackCheck()
{
	//562p 무기 속성 코드 추가 및 변경
	float FinalAttackRange = GetFinalAttackRange();
	//

	FHitResult HitResult;
	FCollisionQueryParams Params(NAME_None, false, this);
	bool bResult = GetWorld()->SweepSingleByChannel(
		HitResult,
		GetActorLocation(),
		GetActorLocation() + GetActorForwardVector() * FinalAttackRange /* 200.0f */ , 
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel2,
		FCollisionShape::MakeSphere(50.0f),
		Params
	);

	// DrawDebug
#if ENABLE_DRAW_DEBUG
	FVector TraceVec = GetActorForwardVector() * FinalAttackRange /* AttackRange */;
	FVector Center = GetActorLocation() + TraceVec * 0.5f;
	float HalfHeight = FinalAttackRange /* AttackRange */ * 0.5f + AttackRadius;
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
		//567p 무기 공격 대미지 속성 값 수정
		HitResult.Actor->TakeDamage(GetFinalAttackDamage()/*CharacterStat->GetAttack()*/,
			DamageEvent, GetController(), this);
	}


}

// 13장 468p 캐릭터 외형 ini 로드한거 랜덤으로 가져오기
void AABCharacter::OnAssetLoadCompleted()
{
	USkeletalMesh* AssetLoaded = Cast<USkeletalMesh>(AssetStreamingHandle->GetLoadedAsset());
	/*
	if (nullptr != AssetLoaded)
	{
		GetMesh()->SetSkeletalMesh(AssetLoaded);
	}
	*/
	// 14장 508p
	AssetStreamingHandle.Reset();
	ABCHECK(nullptr != AssetLoaded);
	GetMesh()->SetSkeletalMesh(AssetLoaded);

	SetCharacterState(ECharacterState::READY);
}

float AABCharacter::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
	float FinalDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	ABLOG(Warning, TEXT("Actor : %s took Damage : %f"), *GetName(), FinalDamage);
	// 이부분 설정은 델리게이트에서 설정
	/*
	if (FinalDamage > 0.0f)
	{
		ABAnim->SetDeadAnim();
		SetActorEnableCollision(false);
	}
	*/
	// 대미지 전달 후 HP <= 0 이면 죽는모션 델리게이트 작동
	 
	CharacterStat->SetDamage(FinalDamage);

	// 경험치 저장 및 레벨변화 536p
	// 플레이어 킬 전달
	if (CurrentState == ECharacterState::DEAD)
	{
		if (EventInstigator->IsPlayerController())
		{
			auto ABPlayerConstroller = Cast<AABPlayerController>(EventInstigator);
			ABCHECK(nullptr != ABPlayerConstroller, 0.0f);
			ABPlayerConstroller->NPCKill(this);
		}
	}

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
