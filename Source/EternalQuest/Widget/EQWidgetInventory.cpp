// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/EQWidgetInventory.h"

#include "EQWidgetItemSlot.h"
#include "Character/EQCharacterPlayer.h"
#include "Components/Button.h"
#include "Components/SizeBox.h"
#include "Components/WidgetSwitcher.h"
#include "Components/WrapBox.h"
#include "Item/EQItemDragDropOperation.h"

UEQWidgetInventory::UEQWidgetInventory(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
	static ConstructorHelpers::FClassFinder<UUserWidget> ItemSlotRef(TEXT("/Game/Blueprints/UI/WBP_Slot.WBP_Slot_C"));
	if (ItemSlotRef.Succeeded())
	{
		ItemSlotFactory = ItemSlotRef.Class;
	}

	static ConstructorHelpers::FObjectFinder<USoundWave> OpenInventorySoundRef(TEXT("/Script/Engine.SoundWave'/Game/Assets/RPG_Interface_SFX/WAV/UI_Menu_01.UI_Menu_01'"));
	if (OpenInventorySoundRef.Succeeded())
	{
		OpenInventorySound = OpenInventorySoundRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<USoundWave> UIBaseSoundRef(TEXT("/Script/Engine.SoundWave'/Game/Assets/RPG_Interface_SFX/WAV/UI_Menu_02.UI_Menu_02'"));
	if (OpenInventorySoundRef.Succeeded())
	{
		UIBaseSound = UIBaseSoundRef.Object;
	}
}

void UEQWidgetInventory::NativeConstruct()
{
	Super::NativeConstruct();
	Btn_Equip->OnClicked.AddDynamic(this, &UEQWidgetInventory::OnClickBtnEquip);
	Btn_Consume->OnClicked.AddDynamic(this, &UEQWidgetInventory::OnClickBtnConsume);
	Btn_Material->OnClicked.AddDynamic(this, &UEQWidgetInventory::OnClickBtnMaterial);
	Btn_QuestItem->OnClicked.AddDynamic(this, &UEQWidgetInventory::OnClickBtnQuestItem);

	// Player = Cast<AEQCharacterPlayer>(GetWorld()->GetFirstPlayerController()->GetPawn());
	// EQComponentInventory = Player->FindComponentByClass<UEQComponentInventory>();
}

bool UEQWidgetInventory::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent,
	UDragDropOperation* InOperation)
{
	UpdateItemInInventoryUI();
	return true;
}

void UEQWidgetInventory::OpenCloseInventoryWidget()
{
	if (bVisibilityFlag)
	{
		bVisibilityFlag = false;
		SizeBox_Base->SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		bVisibilityFlag = true;
		UpdateItemInInventoryUI();
		SizeBox_Base->SetVisibility(ESlateVisibility::Visible);
	}
}

void UEQWidgetInventory::UpdateItemInInventoryUI()
{
	ClearItemInventory();
	AddItemInventory();
}

void UEQWidgetInventory::ClearItemInventory()
{
	WrapBox_Equip->ClearChildren();
	WrapBox_Consume->ClearChildren();
	WrapBox_Material->ClearChildren();
	WrapBox_QuestItem->ClearChildren();
	WrapBox_Equipping->ClearChildren();
}

void UEQWidgetInventory::AddItemInventory()
{
	for (auto& e : GetWorld()->GetFirstPlayerController()->GetPawn()->FindComponentByClass<UEQComponentInventory>()->EQAllItem.Equipment)
	{
		auto temp = CreateWidget(GetWorld(), ItemSlotFactory);
		Cast<UEQWidgetItemSlot>(temp)->EQSlot = &e;
		WrapBox_Equip->AddChildToWrapBox(temp);
	}
	for (auto& e : GetWorld()->GetFirstPlayerController()->GetPawn()->FindComponentByClass<UEQComponentInventory>()->EQAllItem.Consumtion)
	{
		auto temp = CreateWidget(GetWorld(), ItemSlotFactory);
		Cast<UEQWidgetItemSlot>(temp)->EQSlot = &e;
		WrapBox_Consume->AddChildToWrapBox(temp);
	}
	for (auto& e : GetWorld()->GetFirstPlayerController()->GetPawn()->FindComponentByClass<UEQComponentInventory>()->EQAllItem.Material)
	{
		auto temp = CreateWidget(GetWorld(), ItemSlotFactory);
		Cast<UEQWidgetItemSlot>(temp)->EQSlot = &e;
		WrapBox_Material->AddChildToWrapBox(temp);
	}
	for (auto& e : GetWorld()->GetFirstPlayerController()->GetPawn()->FindComponentByClass<UEQComponentInventory>()->EQAllItem.QuestItem)
	{
		auto temp = CreateWidget(GetWorld(), ItemSlotFactory);
		Cast<UEQWidgetItemSlot>(temp)->EQSlot = &e;
		WrapBox_QuestItem->AddChildToWrapBox(temp);
	}
	for (auto& e : GetWorld()->GetFirstPlayerController()->GetPawn()->FindComponentByClass<UEQComponentInventory>()->EQAllItem.EquippingSword)
	{
		auto temp = CreateWidget(GetWorld(), ItemSlotFactory);
		Cast<UEQWidgetItemSlot>(temp)->EQSlot = &e;
		WrapBox_Equipping->AddChildToWrapBox(temp);
	}
	for (auto& e : GetWorld()->GetFirstPlayerController()->GetPawn()->FindComponentByClass<UEQComponentInventory>()->EQAllItem.EquippingShield)
	{
		auto temp = CreateWidget(GetWorld(), ItemSlotFactory);
		Cast<UEQWidgetItemSlot>(temp)->EQSlot = &e;
		WrapBox_Equipping->AddChildToWrapBox(temp);
	}
}

void UEQWidgetInventory::OnClickBtnEquip()
{
	WS_Inventory->SetActiveWidgetIndex(0);
}

void UEQWidgetInventory::OnClickBtnConsume()
{
	WS_Inventory->SetActiveWidgetIndex(1);
}

void UEQWidgetInventory::OnClickBtnMaterial()
{
	WS_Inventory->SetActiveWidgetIndex(2);
}

void UEQWidgetInventory::OnClickBtnQuestItem()
{
	WS_Inventory->SetActiveWidgetIndex(3);
}
