// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Inventory/InventorySlot.h"

#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/InventoryManager.h"
#include "Components/TextBlock.h"
#include "Engine/AssetManager.h"
#include "Engine/StreamableManager.h"

void UInventorySlot::InitSlot(FInventoryItem _Item)
{
	if(!ItemImage || !CountText)
	{
		return;
	}

	Item = _Item;
	if(Item.Count <= 0)
	{
		ClearSlot();
		return;
	}
	
	FStreamableManager& StreamableManager = UAssetManager::Get().GetStreamableManager();
	StreamableManager.RequestAsyncLoad(Item.Item.ToSoftObjectPath(),FStreamableDelegate::CreateWeakLambda(this, [this]()
	{
		if(const UClass* LoadedClass = Item.Item.Get())
		{
			const APickableItem* DefaultObject = LoadedClass->GetDefaultObject<APickableItem>();
			
			ItemImage->SetBrushFromTexture(DefaultObject->GetImage());
			ItemImage->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			
			CountText->SetText(FText::AsNumber(Item.Count));
			CountText->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

			SlotButton->SetVisibility(ESlateVisibility::Visible);
		}
	}));
}

void UInventorySlot::ClearSlot()
{
	Item = FInventoryItem();
	SlotButton->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	ItemImage->SetVisibility(ESlateVisibility::Hidden);
	CountText->SetVisibility(ESlateVisibility::Hidden);
}

void UInventorySlot::NativeConstruct()
{
	Super::NativeConstruct();

	if(SlotButton)
	{
		SlotButton->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		SlotButton->OnClicked.AddUniqueDynamic(this, &ThisClass::OnSlotButtonClicked);
	}

	if(ItemImage)
	{
		ItemImage->SetVisibility(ESlateVisibility::Hidden);
	}

	if(CountText)
	{
		CountText->SetVisibility(ESlateVisibility::Hidden);
	}
}


void UInventorySlot::OnSlotButtonClicked()
{
	OnSlotClicked.Broadcast(this);
}
