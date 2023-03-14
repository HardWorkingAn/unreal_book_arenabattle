// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"

// 스테이트 머신 모델로 게임 진행단계 구분 501p
UENUM(BlueprintType)
enum class ECharacterState : uint8
{
	PREINIT,  //캐릭터 생성 전 캐릭터와 UI를 숨겨둠 해당 스테이트에서는 대미지 피해x
	LOADING, //선택한 캐릭터 에셋을 로딩하는 스테이트, 에셋 로딩 될때 까지 움직임x
	READY, //캐릭터 에셋 로딩이 완료된 스테이트, 숨겨둔 캐릭터와 UI가 보이며 이때부터 움직일 수 있으며 공격 받으면 피해
	DEAD // 캐릭터가 hp를 소진해 사망할 때 발생하는 스테이트 죽는 애니메이션 재생하고 UI를 끄고 충돌 기능 제거
};

DECLARE_LOG_CATEGORY_EXTERN(ArenaBattle, Log, All);

// 예약어가 아닌 지정해준 변수명임
// ABLOG_S : 코드가 들어있는 파일 이름과 함수, 그리고 라인정보를 추가해 ArenaBattle 카테고리로 로그를 남김
// 로그를 사용한 함수의 실행 시점을 파악할  유용
// ABLOG : ABLOG_S 정보에 형식 문자열로 추가 정보를 지정해 로그를 남긴다.
#define ABLOG_CALLINFO (FString(__FUNCTION__) + TEXT("(") + FString::FromInt(__LINE__) + TEXT(")"))
// Verbosity : 로깅수준
#define ABLOG_S(Verbosity) UE_LOG(ArenaBattle, Verbosity, TEXT("%s"), *ABLOG_CALLINFO)
// Printf(*FString::Printf(Format, ##__VA_ARGS__)) 
// ABLOG(Warning, TEXT("Actor Name : %s, ID : %d, Location X : %.3f"), *GetName(), ID, GetActorLocation().X) 에서 나온 Format 매개변수받은 문자열을
// *FString::Printf(Format, ##_VA_ARGS__) Format 이란 매개변수명이 일치하므로 문자열을 가져온다.
// UE_LOG(ArenaBattle, ABLOG 매개변수 Verbosity, TEXT("%s%s"), 
//                                *ABLOG_CALLINFO, "Actor Name ~~ "작동
#define ABLOG(Verbosity, Format, ...) UE_LOG(ArenaBattle, Verbosity, TEXT("%s%s"), *ABLOG_CALLINFO, *FString::Printf(Format, ##__VA_ARGS__))


#define ABCHECK(Expr, ...) { if(!(Expr)) { ABLOG(Error, TEXT("ASSERTION : %s"), TEXT("'"#Expr"'")); return __VA_ARGS__; }}
