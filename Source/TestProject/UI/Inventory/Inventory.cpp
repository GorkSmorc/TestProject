// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Inventory/Inventory.h"

#include "ContextWindow.h"
#include "InventorySlot.h"
#include "Blueprint/SlateBlueprintLibrary.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/InventoryManager.h"
#include "Components/WrapBox.h"

#include "MainClasses/TestProjectPlayerState.h"

void UInventory::NativeConstruct()
{
	Super::NativeConstruct();
	
	GetOwningPlayer()->SetShowMouseCursor(true);
	
	if(Slots.Num() <= 0)
	{
		GenerateSlots();
		return;
	}

	UpdateSlots();
}

void UInventory::NativeDestruct()
{
	if(GetOwningPlayer())
	{
		GetOwningPlayer()->SetShowMouseCursor(false);
	}

	if(ContextWindow)
	{
		ContextWindow->RemoveFromParent();
		ContextWindow = nullptr;
	}
	
	Super::NativeDestruct();
}

void UInventory::ShowContextWindow(UInventorySlot* ClickedSlot)
{
	if(!ContextWindowClass)
	{
		return;
	}

	if(ContextWindow)
	{
		if(ContextWindow->GetClickedSlot() == ClickedSlot)
		{
			ContextWindow->Init(nullptr);
			ContextWindow->RemoveFromParent();
			return;
		}
	}
	else
	{
		ContextWindow = CreateWidget<UContextWindow>(this,ContextWindowClass);
	}
	
	ContextWindow->Init(ClickedSlot);
	FVector2D PixelPosition, ViewPortPosition;
	USlateBlueprintLibrary::LocalToViewport(this,ClickedSlot->GetCachedGeometry(),FVector2D(0),PixelPosition,ViewPortPosition);
	ViewPortPosition *= UWidgetLayoutLibrary::GetViewportScale(this);
	ContextWindow->SetDesiredSizeInViewport(FVector2D(176));
	ContextWindow->SetPositionInViewport(ViewPortPosition + FVector2D(58));
	ContextWindow->AddToViewport();
}

void UInventory::GenerateSlots()
{
	if(!SlotClass)
	{
		return;
	}

	WrapBox->ClearChildren();

	UInventoryManager* InventoryManager = GetOwningPlayerState<ATestProjectPlayerState>()->GetInventoryManager();
	for(size_t i = 0; i < InventoryManager->GetMaxItems(); i++)
	{
		UInventorySlot* NewSlot = CreateWidget<UInventorySlot>(this,SlotClass);
		WrapBox->AddChildToWrapBox(NewSlot);
		Slots.Add(NewSlot);
		
		if(InventoryManager->GetInventory().IsValidIndex(i))
		{
			NewSlot->InitSlot(InventoryManager->GetInventory()[i]);
			NewSlot->OnSlotClicked.AddUniqueDynamic(this, &ThisClass::OnInventorySlotClicked);
		}
	}
	InventoryManager->OnInventoryChanged.AddUniqueDynamic(this, &ThisClass::OnInventoryUpdated);
}

void UInventory::UpdateSlots()
{
	TArray<FInventoryItem> Inventory = GetOwningPlayerState<ATestProjectPlayerState>()->GetInventoryManager()->GetInventory();
	for(size_t i = 0; i < Slots.Num(); i++)
	{
		if(Inventory.IsValidIndex(i))
		{
			Slots[i]->InitSlot(Inventory[i]);
			Slots[i]->OnSlotClicked.AddUniqueDynamic(this, &ThisClass::OnInventorySlotClicked);
		}
		else
		{
			Slots[i]->ClearSlot();
		}
	}
}

void UInventory::OnInventorySlotClicked(UInventorySlot* ClickedSlot)
{
	ShowContextWindow(ClickedSlot);
}

void UInventory::OnInventoryUpdated(int32 ChangedSlotIndex, FInventoryItem NewItem)
{
	if(Slots.IsValidIndex(ChangedSlotIndex))
	{
		Slots[ChangedSlotIndex]->InitSlot(NewItem);
	}
}
