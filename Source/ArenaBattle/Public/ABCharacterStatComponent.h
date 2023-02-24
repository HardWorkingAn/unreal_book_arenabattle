// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ArenaBattle.h"

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ABCharacterStatComponent.generated.h"

// 대미지 전달 후 HP <= 0 이면 죽는모션 위한 델리게이트
DECLARE_MULTICAST_DELEGATE(FOnHPIsZeroDelegate);

// HPBar 위젯이랑 연동하기 위한 델리게이트
DECLARE_MULTICAST_DELEGATE(FOnHPChangedDelegate);


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ARENABATTLE_API UABCharacterStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UABCharacterStatComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	// InitializeComponent() 함수는 PostInitializeComponent 함수 호출하기 전에 호출된다.
	virtual void InitializeComponent() override;

public:
	void SetNewLevel(int32 NewLevel);

	// 대미지 전달 후 HP <= 0 이면 죽는모션
	void SetDamage(float NewDamage);
	void SetHP(float NewHP);
	float GetAttack();
	float GetHPRatio();
	
	// 경험치 저장 및 레벨변화 532p
	int32 GetDropExp() const;
	//

	FOnHPIsZeroDelegate OnHPIsZero;

	FOnHPChangedDelegate OnHPChanged;

private:
	struct FABCharacterData* CurrentStatData = nullptr;

	UPROPERTY(EditInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = true))
	int32 Level;

	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = true))
	float CurrentHP;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
