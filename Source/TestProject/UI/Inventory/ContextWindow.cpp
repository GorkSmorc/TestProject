// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Inventory/ContextWindow.h"

#include "InventorySlot.h"
#include "Components/Button.h"
#include "Components/InventoryManager.h"
#include "MainClasses/TestProjectPlayerState.h"
#include "Utilities/UseInterface.h"

void UContextWindow::NativeConstruct()
{
	Super::NativeConstruct();

	if(UseButton)
		UseButton->OnClicked.AddUniqueDynamic(this, &ThisClass::OnUseItem);

	if(DeleteButton)
		DeleteButton->OnClicked.AddUniqueDynamic(this, &ThisClass::OnDeleteItem);
	
}

void UContextWindow::Init(UInventorySlot* InventorySlot)
{
	ClickedSlot = InventorySlot;
	if(ClickedSlot)
	{
		if(ClickedSlot->GetItem().Item->ImplementsInterface(UUseInterface::StaticClass()))
		{
			UseButton->SetVisibility(ESlateVisibility::Visible);
		}
		else
		{
			UseButton->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}

void UContextWindow::OnUseItem()
{
	if(ClickedSlot)
	{
		FActorSpawnParameters SpawnParameters;
		SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		
		AActor* UsableActor = GetWorld()->SpawnActor<AActor>(ClickedSlot->GetItem().Item.LoadSynchronous(),SpawnParameters);
		if(IUseInterface::Execute_Use(UsableActor))
		{
			GetOwningPlayerState<ATestProjectPlayerState>()->GetInventoryManager()->RemoveItem(ClickedSlot->GetItem().Item.Get(),1);
		}
	}
	
	ClickedSlot = nullptr;
	RemoveFromParent();
}

void UContextWindow::OnDeleteItem()
{
	if(ClickedSlot)
	{
		GetOwningPlayerState<ATestProjectPlayerState>()->GetInventoryManager()->RemoveItem(ClickedSlot->GetItem().Item.Get(), ClickedSlot->GetItem().Count);
		ClickedSlot = nullptr;
		RemoveFromParent();
	}
}
