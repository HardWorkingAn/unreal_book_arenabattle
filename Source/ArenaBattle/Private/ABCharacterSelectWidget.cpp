// Fill out your copyright notice in the Description page of Project Settings.


#include "ABCharacterSelectWidget.h"

// 588p
#include "ABCharacterSetting.h"
#include "ABGameInstance.h"
#include "EngineUtils.h"
#include "Animation/SkeletalMeshActor.h"

// 590p
#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "ABSaveGame.h"
#include "ABPlayerState.h"

void UABCharacterSelectWidget::NextCharacter(bool bForward)
{
	ABLOG(Warning, TEXT("OnNextCharacter()"));
	bForward ? CurrentIndex++ : CurrentIndex--;

	// 캐릭터의 모델 Index -1되면 Max 맨마지막 이동 MaxIndex 되면 첫번쨰 모델
	if (CurrentIndex == -1) CurrentIndex = MaxIndex - 1;
	if (CurrentIndex == MaxIndex) CurrentIndex = 0;
	// ini 파일로 가져온 데이터
	auto CharacterSettig = GetDefault<UABCharacterSetting>();

	auto AssetRef = CharacterSettig->CharacterAssets[CurrentIndex];

	auto ABGameInstance = GetWorld()->GetGameInstance<UABGameInstance>();
	ABCHECK(nullptr != ABGameInstance);
	ABCHECK(TargetComponent.IsValid());

	USkeletalMesh* Asset = ABGameInstance->StreamableManager.LoadSynchronous<USkeletalMesh>(AssetRef);
	if (nullptr != Asset)
	{
		TargetComponent->SetSkeletalMesh(Asset);
	}

}


void UABCharacterSelectWidget::NativeConstruct()
{
	Super::NativeConstruct();

	ABLOG(Warning, TEXT("NativeConstruct()"));

	CurrentIndex = 0;
	auto CharacterSetting = GetDefault<UABCharacterSetting>();
	MaxIndex = CharacterSetting->CharacterAssets.Num();

	for (TActorIterator<ASkeletalMeshActor> It(GetWorld()); It; ++It)
	{
		TargetComponent = It->GetSkeletalMeshComponent();
		break;
	}

	// 590p UI 와 변수 간의 연결(바인딩)
	PrevButton = Cast<UButton>(GetWidgetFromName(TEXT("btnPrev")));
	ABCHECK(nullptr != PrevButton);

	NextButton = Cast<UButton>(GetWidgetFromName(TEXT("btnNext")));
	ABCHECK(nullptr != NextButton);

	ConfirmButton = Cast<UButton>(GetWidgetFromName(TEXT("btnConfirm")));
	ABCHECK(nullptr != ConfirmButton);

	TextBox = Cast<UEditableTextBox>(GetWidgetFromName(TEXT("edtPlayerName")));
	ABCHECK(nullptr != TextBox);

	// 클릭 델리게이트에 함수 바인딩
	PrevButton->OnClicked.AddDynamic(this, &UABCharacterSelectWidget::OnPrevClicked);
	NextButton->OnClicked.AddDynamic(this, &UABCharacterSelectWidget::OnNextClicked);
	ConfirmButton->OnClicked.AddDynamic(this, &UABCharacterSelectWidget::OnConfirmClicked);
}

void UABCharacterSelectWidget::OnPrevClicked()
{
	ABLOG(Warning, TEXT("OnPrevClicked()"));
	NextCharacter(false);
}

void UABCharacterSelectWidget::OnNextClicked()
{
	ABLOG(Warning, TEXT("OnNextClicked()"));
	NextCharacter(true);
}

void UABCharacterSelectWidget::OnConfirmClicked()
{
	ABLOG(Warning, TEXT("OnConfirmClicked()"));
	FString CharacterName = TextBox->GetText().ToString();

	// 글자가 없거나 10글자 초과할 경우
	if (CharacterName.Len() <= 0 || CharacterName.Len() > 10)
	{
		return; // 리턴을 아무것도 안하여 여기서 함수종료
	}

	UABSaveGame* NewPlayerData = NewObject<UABSaveGame>();
	NewPlayerData->PlayerName = CharacterName;
	NewPlayerData->Level = 1;
	NewPlayerData->Exp = 0;
	NewPlayerData->HighScore = 0;

	// GetDefault()는 UClass를 상속받은 클래스에서 사용할 수 있는 함수
	auto ABPlayerState = GetDefault<AABPlayerState>();

	if (UGameplayStatics::SaveGameToSlot(NewPlayerData, ABPlayerState->SaveSlotName, 0))
	{
		// 게임 Level(화면,배경) 전환
		UGameplayStatics::OpenLevel(GetWorld(), TEXT("Gameplay"));
	}
	else
	{
		ABLOG(Error, TEXT("SaveGame Error"));
	}

}
