// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ABCharacterSetting.generated.h"

// ABCharacterSetting  앞으로 사용할 캐릭터 에셋의 목록을 보관.
// 생성자 코드에 캐릭터 에셋을 코드로 지정할 수 있지만 만일 에셋이 변경되면 코드를 다시 만들고 컴파일 해야 하기 때문에
// 언리얼 오브젝트의 기본값을 유연하게 관리 하도록 외부 INI파일 에서 기본 속성값을 지정하는 기능 사용
/**
 * 
 */
// 언리얼에 기본값인 INI파일 불러들일려면 UCLASS에 config 키워드를 추가해 여기에 불러들일 INI파일 이름을 지정하고 
// 불러들일 PROPERTY 속성에 config 키워드를 선언해야한다.
UCLASS(config=ArenaBattle) // config 에서 Arenabattle를 지정해서 config 폴더 안에 있는 DefaultArenaBattle.ini 파일을 읽어온다.
class ARENABATTLESETTING_API UABCharacterSetting : public UObject
{
	GENERATED_BODY()
	
public:
	UABCharacterSetting();

	UPROPERTY(config)
	//에셋 경로 위치를 저장 보관하는 FSoftObjectPath
	TArray<FSoftObjectPath> CharacterAssets;

};
