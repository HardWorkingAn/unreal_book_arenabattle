// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ArenaBattle.h"

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ABPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class ARENABATTLE_API AABPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	AABPlayerController();

	// Actor ������Ʈ�� ������ �ʱ�ȭ�� ���� ȣ��ȴ�. ���� �÷��� �߿��� ȣ��
	virtual void PostInitializeComponents() override;
	// OnPossess �� ��Ʈ�귯�� ���� ����
	virtual void OnPossess(APawn* aPawn) override;
	// 4.22���� ���ĺ��ʹ� Possess�� �������̵� �� �� �� ������ OnPossess�� �ٲ�
	//virtual void Possess(APawn* aPawn) override;

	class UABHUDWidget* GetHUDWidget() const;
	// ����ġ ���� �� ������ȭ 536p
	void NPCKill(class AABCharacter* KilledNPC) const;
	// 546p
	void AddGameScore() const;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = UI)
	TSubclassOf<class UABHUDWidget> HUDWidgetClass;

private:
	UPROPERTY()
	class UABHUDWidget* HUDWidget;

	// ����ġ ���� �� ������ȭ 536p
	UPROPERTY()
	class AABPlayerState* ABPlayerState;
};
