// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ArenaBattle.h"

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ABCharacterSelectWidget.generated.h"

/**
 * 
 */
UCLASS()
class ARENABATTLE_API UABCharacterSelectWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	// NextCharacter 함수는 블루 프린트에서 사용 가능하게 UFUNCTION 매크로 추가후
	// BlueprintCallable(Blueprint Call Able) 키워드 추가
	// 하지만 예제에서 작성한건 C++로 직접 바인딩 함 ※블루프린트로도 바인딩가능
	UFUNCTION(BlueprintCallable)
	void NextCharacter(bool bForward = true);

	//위젯이 렌더링되기 전에 수행해야 하는 초기화 작업을 담당하는 함수입니다.
	virtual void NativeConstruct() override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Character)
	int32 CurrentIndex;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Character)
	int32 MaxIndex;

	// 약한 참조란, 포인터가 가리키는 객체가 소멸되었을 때 자동으로 null로 설정되는 참조
	TWeakObjectPtr<USkeletalMeshComponent> TargetComponent;

	// 클릭 버튼
	UPROPERTY()
	class UButton* PrevButton;

	UPROPERTY()
	class UButton* NextButton;

	UPROPERTY()
	class UButton* ConfirmButton;
	// 수정가능한 텍스트박스
	UPROPERTY()
	class UEditableTextBox* TextBox;

private:
	// 클릭하면 작동되는 함수
	UFUNCTION()
	void OnPrevClicked();
	UFUNCTION()
	void OnNextClicked();
	UFUNCTION()
	void OnConfirmClicked();

};
