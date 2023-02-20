// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"

// ������Ʈ �ӽ� �𵨷� ���� ����ܰ� ���� 501p
UENUM(BlueprintType)
enum class ECharacterState : uint8
{
	PREINIT,  //ĳ���� ���� �� ĳ���Ϳ� UI�� ���ܵ� �ش� ������Ʈ������ ����� ����x
	LOADING, //������ ĳ���� ������ �ε��ϴ� ������Ʈ, ���� �ε� �ɶ� ���� ������x
	READY, //ĳ���� ���� �ε��� �Ϸ�� ������Ʈ, ���ܵ� ĳ���Ϳ� UI�� ���̸� �̶����� ������ �� ������ ���� ������ ����
	DEAD // ĳ���Ͱ� hp�� ������ ����� �� �߻��ϴ� ������Ʈ �״� �ִϸ��̼� ����ϰ� UI�� ���� �浹 ��� ����
};

DECLARE_LOG_CATEGORY_EXTERN(ArenaBattle, Log, All);

// ���� �ƴ� �������� ��������
// ABLOG_S : �ڵ尡 ����ִ� ���� �̸��� �Լ�, �׸��� ���������� �߰��� ArenaBattle ī�װ��� �α׸� ����
// �α׸� ����� �Լ��� ���� ������ �ľ��� �� ����
// ABLOG : ABLOG_S ������ ���� ���ڿ��� �߰� ������ ������ �α׸� �����.
#define ABLOG_CALLINFO (FString(__FUNCTION__) + TEXT("(") + FString::FromInt(__LINE__) + TEXT(")"))
// Verbosity : �α����
#define ABLOG_S(Verbosity) UE_LOG(ArenaBattle, Verbosity, TEXT("%s"), *ABLOG_CALLINFO)
// Printf(*FString::Printf(Format, ##__VA_ARGS__)) 
// ABLOG(Warning, TEXT("Actor Name : %s, ID : %d, Location X : %.3f"), *GetName(), ID, GetActorLocation().X) ���� ���� Format �Ű��������� ���ڿ���
// *FString::Printf(Format, ##_VA_ARGS__) Format �̶� �Ű��������� ��ġ�ϹǷ� ���ڿ��� �����´�.
// UE_LOG(ArenaBattle, ABLOG �Ű����� Verbosity, TEXT("%s%s"), 
//                                *ABLOG_CALLINFO, "Actor Name ~~ "�۵�
#define ABLOG(Verbosity, Format, ...) UE_LOG(ArenaBattle, Verbosity, TEXT("%s%s"), *ABLOG_CALLINFO, *FString::Printf(Format, ##__VA_ARGS__))


#define ABCHECK(Expr, ...) { if(!(Expr)) { ABLOG(Error, TEXT("ASSERTION : %s"), TEXT("'"#Expr"'")); return __VA_ARGS__; }}