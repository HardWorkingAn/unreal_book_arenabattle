// Fill out your copyright notice in the Description page of Project Settings.


#include "ABGameInstance.h"

// ĳ���� ������ csv �� ������ ����
// ��Ȯ�� ������ �ش�����

UABGameInstance::UABGameInstance()
{
	FString CharacterDataFath = TEXT("DataTable'/Game/Book/GameData/ABCharacterData.ABCharacterData'");
	static ConstructorHelpers::FObjectFinder<UDataTable> DT_ABCHARACTER(*CharacterDataFath);
	ABCHECK(DT_ABCHARACTER.Succeeded());
	ABCharacterTable = DT_ABCHARACTER.Object;
	ABCHECK(ABCharacterTable->GetRowMap().Num() > 0);
}

void UABGameInstance::Init()
{
	Super::Init();

	ABLOG(Warning, TEXT("DropExp of Level 20 ABCharactor : %d"), GetABCharacterData(20)->DropExp);
}

FABCharacterData* UABGameInstance::GetABCharacterData(int32 Level)
{
	return ABCharacterTable->FindRow<FABCharacterData>(*FString::FromInt(Level), TEXT(""));
}
