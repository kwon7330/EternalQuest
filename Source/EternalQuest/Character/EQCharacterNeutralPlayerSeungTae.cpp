// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/EQCharacterNeutralPlayerSeungTae.h"

#include "NiagaraComponent.h"
#include "Blueprint/UserWidget.h"
#include "Component/EQComponentQuest.h"
#include "Item/EQItemBase.h"

AEQCharacterNeutralPlayerSeungTae::AEQCharacterNeutralPlayerSeungTae()
{
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> MeshRef(TEXT("/Game/Assets/Cute_Little_Animals/Piggy/Mesh/SK_Piggy.SK_Piggy"));
	if (MeshRef.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(MeshRef.Object);
	}

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> ClothMeshRef(TEXT("/Game/Assets/Cute_Little_Animals/Piggy/Mesh/SK_Piggy_Clothes.SK_Piggy_Clothes"));
	if (ClothMeshRef.Succeeded())
	{
		ClothComp->SetSkeletalMesh(ClothMeshRef.Object);
	}
}

void AEQCharacterNeutralPlayerSeungTae::BeginPlay()
{
	Super::BeginPlay();
	
	NPCName = TEXT("승태");
	
	NPCPrompt.PushLast(TEXT("안녕!, 넌 엘리시움에 못보던 얼굴인데 모험가인가봐?"));
	NPCPrompt.PushLast(TEXT("나는 새싹광산에서 일하고 있는 승태 라고 해,"));
	NPCPrompt.PushLast(TEXT("일기장에 일기와 오늘의 할일을 써야 하는데 어디있는지 보이질 않어!"));
	NPCPrompt.PushLast(TEXT("근처에 떨어뜨린 것 같은데, 혹시 네가 찾아줄 수 있어? QuestTag"));
}

void AEQCharacterNeutralPlayerSeungTae::QuestAccepted()
{
	NPCPrompt.Reset();
	if (GetWorld()->GetFirstPlayerController()->GetCharacter()->FindComponentByClass<UEQComponentQuest>()->QuestCondition.QuestStateArray[0] == 0)
	{
		GetWorld()->GetFirstPlayerController()->GetCharacter()->FindComponentByClass<UEQComponentQuest>()->QuestCondition.QuestStateArray[0] = 1;
	}
	NPCPrompt.PushLast(TEXT("혹시 내 일기장을 못봤니?"));
	QuestIcon->SetVisibility(false);
}

void AEQCharacterNeutralPlayerSeungTae::QuestDenied()
{
}

void AEQCharacterNeutralPlayerSeungTae::QuestSuccess()
{
	if (GetWorld()->GetFirstPlayerController()->GetCharacter()->FindComponentByClass<UEQComponentQuest>()->QuestCondition.QuestStateArray[0] == 2)
	{
		NPCPrompt.Reset();
		NPCPrompt.PushLast(TEXT("역시 햄버거보다는 국밥이지~"));
	}
	
	if (GetWorld()->GetFirstPlayerController()->GetCharacter()->FindComponentByClass<UEQComponentQuest>()->QuestCondition.QuestStateArray[0] != 1)
	{
		Super::QuestSuccess();
		return;
	}
	
	if (GetWorld()->GetFirstPlayerController()->GetCharacter()->FindComponentByClass<UEQComponentQuest>()->QuestCondition.bQuestCond1 != true)
	{
		Super::QuestSuccess();
		return;
	}
	
	NPCPrompt.Reset();
	NPCPrompt.PushLast(TEXT("내 일기장을 찾아주었구나! 고마워, 보상으로 너에게 어울리는 무기를 줄게."));
	auto CurrItem = GetWorld()->SpawnActorDeferred<AEQItemBase>(SpawnItemFactory, GetActorTransform());
	if (CurrItem)
	{
		CurrItem->ItemName.DataTable = ItemDataTable;
		CurrItem->ItemName.RowName = TEXT("Forsaken_Wand");
		CurrItem->ItemType = EEQItemType::Equipment;
		CurrItem->ItemQuantity = 1;
	}
	CurrItem->FinishSpawning(GetActorTransform());

	GetWorld()->GetFirstPlayerController()->GetCharacter()->FindComponentByClass<UEQComponentQuest>()->QuestCondition.QuestStateArray[0] = 2;
	
	Super::QuestSuccess();
}
