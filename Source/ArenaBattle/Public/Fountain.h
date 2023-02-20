// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ArenaBattle.h"

#include "EngineMinimal.h"
//움직임 관련 해더파일
#include "GameFramework/RotatingMovementComponent.h"

#include "GameFramework/Actor.h"
#include "Fountain.generated.h"



UCLASS()
class ARENABATTLE_API AFountain : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFountain();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void PostInitializeComponents() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* Body;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* Water;

	UPROPERTY(VisibleAnywhere)
	UPointLightComponent* Light;

	UPROPERTY(VisibleAnywhere)
	UParticleSystemComponent* Splash;

	UPROPERTY(EditAnywhere, Category=ID)
	int32 ID;

	// 115p 무브먼트 컴포넌트 #include "GameFramework/RotatingMovementComponent.h" 같이 필요한것을 참조해야함
	// 4가지 : FloatingPawnMovement : 중력의 영향을 받지 않는 액터의 움직임
	//         RotatingMoveMent     : 지정한 속도로 엑터를 회전
	//         InterpMovement		: 지정한 위치로 엑터를 이동
	//		   ProjectileMovement	: 엑터에 중력의 영향을 받아 포물선을 그리는 발사체 움직임 제공. 주로 총알이나 미사일 사용

	UPROPERTY(VisibleAnywhere)
	URotatingMovementComponent* Movement;

private:
	UPROPERTY(EditAnyWhere, Category = Stat, Meta = (AllowPrivateAccess = true))
	float RotateSpeed;
};
