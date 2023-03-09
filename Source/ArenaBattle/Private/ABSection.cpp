// Fill out your copyright notice in the Description page of Project Settings.


#include "ABSection.h"

// NPC 리젠과 아이템 리젠에 필요한 헤더
#include "ABCharacter.h"
#include "ABItemBox.h"

// 544p 추가
#include "ABPlayerController.h"
// 549p 추가
#include "ABGameMode.h"

// Sets default values
AABSection::AABSection()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// 메쉬 생성 및 최상위 계층(루트) 메쉬 설정
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MESH"));
	RootComponent = Mesh;

	// 기존 SM_SQUARE 맵에는 각 방향별 출입문 과 섹션을 이어 붙일 수 있게 여덟개의 소켓이 부착되어 있다.
	FString AssetPath = TEXT("/Game/Book/StaticMesh/SM_SQUARE.SM_SQUARE");
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_SQUARE(*AssetPath);
	if (SM_SQUARE.Succeeded())
	{
		Mesh->SetStaticMesh(SM_SQUARE.Object);
	}
	else
	{
		ABLOG(Error, TEXT("Failed to load staticmesh asset. : %s"), *AssetPath);
	}

	// 프로젝트 콜리전 에서 추가한 Trigger 설정
	// 박스모양 컴포넌트 생성 후 문에 부착
	Trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("TRIGGER"));
	Trigger->SetBoxExtent(FVector(775.0f, 775.0f, 300.0f));
	Trigger->SetupAttachment(RootComponent);
	Trigger->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	Trigger->SetCollisionProfileName(TEXT("ABTrigger"));

	// 새로운 맵 생성된후 입장하면 그 맵은 State는 READY 상태에서 입장 트리거 발동되면 
	// BATTLE 상태로 변경되어 잠시동안 콜리전을 NoCollision으로 변경하여 트리거 작동 안되게 변경
	// 소환된 AI가 죽어 OnKeyNPCDestroyed() 함수가 작동되면 해당 맵 Actor 는 COMPLETE가 되어
	// Trigger는 NoColision 그대로 유지하고 GateTrigger만 콜리전을 ABTrigger 콜리전으로 변경하여
	// 맵이동 가능.
	Trigger->OnComponentBeginOverlap.AddDynamic(this, &AABSection::OnTriggerBeginOverlap);

	FString GateAssetPath = TEXT("/Game/Book/StaticMesh/SM_GATE.SM_GATE");
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_GATE(*GateAssetPath);
	if (!SM_GATE.Succeeded())
	{
		ABLOG(Error, TEXT("Failed to load staticmesh asset. : %s"), *GateAssetPath);
	}

	static FName GateSockets[] = { TEXT("+XGate"), TEXT("-XGate"), TEXT("+YGate") ,TEXT("-YGate") };
	for (FName GateSocket : GateSockets)
	{
		// DoesSocketExist : 주어진 이름의 소켓이 존재하면 true를 반환합니다.
		ABCHECK(Mesh->DoesSocketExist(GateSocket));
		UStaticMeshComponent* NewGate = CreateDefaultSubobject<UStaticMeshComponent>(*GateSocket.ToString());
		NewGate->SetStaticMesh(SM_GATE.Object);
		NewGate->SetupAttachment(RootComponent, GateSocket);
		NewGate->SetRelativeLocation(FVector(0.0f, -80.5f, 0.0f));
		GateMeshes.Add(NewGate);

		UBoxComponent* NewGateTrigger = CreateDefaultSubobject<UBoxComponent>(*GateSocket.ToString().Append(TEXT("Trigger")));
		NewGateTrigger->SetBoxExtent(FVector(100.0f, 100.0f, 300.0f));
		NewGateTrigger->SetupAttachment(RootComponent, GateSocket);
		NewGateTrigger->SetRelativeLocation(FVector(70.0f, 0.0f, 250.0f));
		NewGateTrigger->SetCollisionProfileName(TEXT("ABTrigger"));
		GateTriggers.Add(NewGateTrigger);
		// 문에 있는 투명 Box Component 와 캐릭터와 충돌하면 OnComponentBeginOverlap 호출
		// 해당 위치에 맵 Actor 있는지 확인후 없으면 새로운 맵 Actor 생성
		NewGateTrigger->OnComponentBeginOverlap.AddDynamic(this, &AABSection::OnGateTriggerBeginOverlap);
		NewGateTrigger->ComponentTags.Add(GateSocket);
		// 개인확인용 추가
		UE_LOG(LogTemp, Warning, TEXT("%s : %s"), *GateSocket.ToString(), *GateSocket.ToString().Append("Trigger"));
	}
	bNoBattle = false;

	// 스폰 타이머 값 설정
	EnemySpawnTime = 2.0f;
	ItemBoxSpawnTime = 5.0f;
}

// Called when the game starts or when spawned
void AABSection::BeginPlay()
{
	Super::BeginPlay();

	SetState(bNoBattle ? ESectionState::COMPLETE : ESectionState::READY);
}

// Called every frame
void AABSection::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AABSection::SetState(ESectionState NewState)
{
	UE_LOG(LogTemp, Warning, TEXT("SetState"));
	switch (NewState)
	{
	case ESectionState::READY:
	{
		Trigger->SetCollisionProfileName(TEXT("ABTrigger"));
		for (UBoxComponent* GateTrigger : GateTriggers)
		{
			GateTrigger->SetCollisionProfileName(TEXT("NoCollision"));
		}

		OperateGates(true);
		break;
	}
	case ESectionState::BATTLE:
	{
		Trigger->SetCollisionProfileName(TEXT("NoCollision"));
		for (UBoxComponent* GateTrigger : GateTriggers)
		{
			GateTrigger->SetCollisionProfileName(TEXT("NoCollision"));
		}
		OperateGates(false);

		// 스폰 설정
		// NPC 생성OnNPCSpawn() 함수를 이용작동하여 NPC를 스폰
		GetWorld()->GetTimerManager().SetTimer(SpawnNPCTimerHandle, FTimerDelegate::CreateUObject(this, &AABSection::OnNPCSpawn), EnemySpawnTime, false);
		GetWorld()->GetTimerManager().SetTimer(SpawnItemBoxTimerHandle, FTimerDelegate::CreateLambda([this]()-> void
			{
				FVector2D RandXY = FMath::RandPointInCircle(600.0f);
				GetWorld()->SpawnActor<AABItemBox>(GetActorLocation() + FVector(RandXY, 30.0f), FRotator::ZeroRotator);

			}),ItemBoxSpawnTime, false);
		break;
	}
	case ESectionState::COMPLETE:
	{
		// 해당 맵을 클리어 하면 맵 Actor는 NoCollision으로 변경하여 작동 안되게 변경
		Trigger->SetCollisionProfileName(TEXT("NoCollision"));
		for (UBoxComponent* GateTrigger : GateTriggers)
		{
			GateTrigger->SetCollisionProfileName(TEXT("ABTrigger"));
			//UE_LOG(LogTemp, Warning, TEXT("%s"), GateTrigger->GetCollisionProfileName().ToString());
		}
		OperateGates(true);
		break;
	}
	}
	
	CurrentState = NewState;
}

// 게이트 각도 변경 -90 은 열린상태 ZeroRotator은 닫힌상태
void AABSection::OperateGates(bool bOpen)
{
	UE_LOG(LogTemp, Warning, TEXT("OperateGates(게이트 각도 변경)"));
	for (UStaticMeshComponent* Gate : GateMeshes)
	{
		Gate->SetRelativeRotation(bOpen ? FRotator(0.0f, -90.0f, 0.0f) : FRotator::ZeroRotator);
	}
}

void AABSection::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	SetState(bNoBattle ? ESectionState::COMPLETE : ESectionState::READY);
}


void AABSection::OnTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("OnTriggerBeginOverlap"));
	if (CurrentState == ESectionState::READY)
	{
		//ABLOG(Warning, TEXT("OnTriggerBeginOverlap : Ready -> Battle "));
		SetState(ESectionState::BATTLE);
	}
}

UFUNCTION()
void AABSection::OnGateTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("OnGateTriggerBeginOverlap"));
	ABCHECK(OverlappedComponent->ComponentTags.Num() == 1);

	// 생성자에서 각 투명 Component 에는 소켓 이름으로 Tag를 저장했음
	FName ComponentTag = OverlappedComponent->ComponentTags[0];
	// 닿은 Component의 +XGate +YGate 이렇게 저장된 이름에서 왼쪽 2개만 가져와 +X +Y같은 값을 가지게됨
	FName SocketName = FName(*ComponentTag.ToString().Left(2));
	if (!Mesh->DoesSocketExist(SocketName))
		return;
	
	// 맵에 잘보면 +X +Y -X -Y 소켓이 따로있으며 이 소켓은 중앙에서 각 -1852 1852 같은 값을 가지고 있다.
	// 하지만 소켓의 좌표값은 상대적이기 떄문에
	// x -1852 y 0 z 0 위치해 있는 Actor에서 -x의 소켓 위치는 -1852 - 1852 인 -3704 위치를 가지게 된다.
	FVector NewLocation = Mesh->GetSocketLocation(SocketName);

	TArray<FOverlapResult> OverlapResults;
	// 검사에서 제외될 Actor 목록
	FCollisionQueryParams CollisionQueryParam(NAME_None, false, this);
	FCollisionObjectQueryParams ObjectQueryParam(FCollisionObjectQueryParams::InitType::AllObjects);

	bool bResult = GetWorld()->OverlapMultiByObjectType(
		OverlapResults, // 만약 충돌한다면 충돌한 actor를 overlapResults 에 저장
		NewLocation, // 상대적 소켓위치
		FQuat::Identity, // 충돌 검사 당하는 액터를 전부 회전값 0으로 보고 검사
		ObjectQueryParam, // 충돌하는 오브젝트 타입은 전부
		FCollisionShape::MakeSphere(775.0f), // 반지름 775인 원 생성후 충돌검사
		CollisionQueryParam // 검사에서 제외될 Actor 목록
	);

	if (!bResult)
	{
		auto NewSection = GetWorld()->SpawnActor<AABSection>(NewLocation, FRotator::ZeroRotator);
	}
	else
	{
		ABLOG(Warning, TEXT("New section area is not empty."));
	}
}

void AABSection::OnNPCSpawn()
{	
	// 544p 에서 코드 수정
	// 새로운 맵 들어가면 NPC 생성
	// GetWorld()->SpawnActor<AABCharacter>(GetActorLocation() + FVector::UpVector * 88.0f, FRotator::ZeroRotator);

	//  현재 액터가 존재하는 월드의 타이머 매니저에서 이전에 생성된 SpawnNPCTimerHandle 타이머를 제거하는 코드

	// GetWorld()->GetTimerManager().ClearTimer(SpawnNPCTimerHandle);

	auto KeyNPC = GetWorld()->SpawnActor<AABCharacter>(GetActorLocation() + FVector::UpVector * 88.0f, FRotator::ZeroRotator);
	if (nullptr != KeyNPC)
	{
		KeyNPC->OnDestroyed.AddDynamic(this, &AABSection::OnKeyNPCDestroyed);
	}
}

void AABSection::OnKeyNPCDestroyed(AActor* DestroyedActor)
{
	ABLOG(Warning, TEXT("OnKeyNPCDestroyed 개인 체크 로그"));

	auto ABCharacter = Cast<AABCharacter>(DestroyedActor);
	ABCHECK(nullptr != ABCharacter);

	// LastHitBy는 마지막으로 어떤 공격자에게 공격을 받았는지 저장하는 변수
	auto ABPlayerController = Cast<AABPlayerController>(ABCharacter->LastHitBy);
	ABCHECK(nullptr != ABPlayerController);
	//549p 추가
	// GetAuthGameMode : 현재 실행 중인 서버에서 사용 중인 게임 모드 클래스를 반환하는 함수
	auto ABGameMode = Cast<AABGameMode>(GetWorld()->GetAuthGameMode());
	ABCHECK(nullptr != ABGameMode);
	ABGameMode->AddScore(ABPlayerController);

	SetState(ESectionState::COMPLETE);
}

