// Fill out your copyright notice in the Description page of Project Settings.

#include "Fountain.h"




// Sets default values
AFountain::AFountain()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bCanEverTick = false;

	// ������Ʈ ����
	Body = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BODY"));
	Water = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WATER"));
	Light = CreateDefaultSubobject<UPointLightComponent>(TEXT("LIGHT"));
	Splash = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("SPLASH"));
	Movement = CreateDefaultSubobject<URotatingMovementComponent>(TEXT("MOVEMENT"));
	
	// ������Ʈ �θ�Root ����
	RootComponent = Body;
	// Body������Ʈ �ڽ����� �߰�
	Water->SetupAttachment(Body);
	Light->SetupAttachment(Body);
	Splash->SetupAttachment(Body);
	// ��ġ ����
	Water->SetRelativeLocation(FVector(0.0f, 0.0f, 135.0f));
	Light->SetRelativeLocation(FVector(0.0f, 0.0f, 195.0f));
	Splash->SetRelativeLocation(FVector(0.0f, 0.0f, 195.0f));
	// �м� ���ӵ�
	RotateSpeed = 30.0f;
	// ������Ʈ�� �̿��Ͽ� tick �� ������� �ʰ� �Ȱ��� ȸ�� ��ų �� �ִ�.
	// �� �κ� ó�� StaticMesh ������Ʈ ó�� Ʈ������ ������ �ʼ����� ������Ʈ�� �� ������Ʈ��� �ϰ�
	// ������ ���� ������Ʈ�� �׼� ������Ʈ��� �Ѵ�.
	Movement->RotationRate = FRotator(0.0f, RotateSpeed, 0.0f);


	// SM_BODY ������ ������Ʈ ������ ��������� ������ġ/���ϸ��� �����ϴ°�
	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		SM_BODY(TEXT("/Game/InfinityBladeGrassLands/Environments/Plains/Env_Plains_Ruins/StaticMesh/SM_Plains_Castle_Fountain_01.SM_Plains_Castle_Fountain_01"));

	
	if (SM_BODY.Succeeded())
	{
		// Fountain�� ������Ʈ�� �巡�� �� �� �ڵ������� SM_BODY ������Ʈ�� ��� (Fountain �巡�� �ؼ� ������ ���� Ȯ�� ����
		Body->SetStaticMesh(SM_BODY.Object);
	}

	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		SM_WATER(TEXT("/Game/InfinityBladeGrassLands/Effects/FX_Meshes/Env/SM_Plains_Fountain_02.SM_Plains_Fountain_02"));

	if (SM_WATER.Succeeded())
	{
		Water->SetStaticMesh(SM_WATER.Object);
	}

	static ConstructorHelpers::FObjectFinder<UParticleSystem>
		PS_SPLASH(TEXT("/Game/InfinityBladeGrassLands/Effects/FX_Ambient/Water/P_Water_Fountain_Splash_Base_01.P_Water_Fountain_Splash_Base_01"));

	if (PS_SPLASH.Succeeded())
	{
		Splash->SetTemplate(PS_SPLASH.Object);
	}

	
}

// Called when the game starts or when spawned
void AFountain::BeginPlay()
{
	Super::BeginPlay();
	//UE_LOG(ArenaBattle, Warning, TEXT("Actor Name : %s, ID : %d, Location x = %.3f"), *GetName(), ID, GetActorLocation().X);
	ABLOG_S(Warning);
	ABLOG(Warning, TEXT("Actor Name : %s, ID : %d, Location X : %.3f"), *GetName(), ID, GetActorLocation().X);
}

void AFountain::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	ABLOG_S(Warning);
}

void AFountain::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	ABLOG_S(Warning);
}

// Called every frame
void AFountain::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//�����Ӹ��� ��������
	//AddActorLocalRotation(FRotator(X ��, Z ��, Y��)); ��ġ�� �ƴ� ȸ��
	//AddActorLocalRotation(FRotator(0.0f, RotateSpeed * DeltaTime, 0.0f));
}