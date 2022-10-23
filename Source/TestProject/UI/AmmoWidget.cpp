// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/AmmoWidget.h"

#include "Components/InventoryManager.h"
#include "Components/TextBlock.h"
#include "GameObjects/Characters/TestProjectCharacter.h"
#include "GameObjects/Items/Weapons/GeneralWeapon.h"
#include "MainClasses/TestProjectPlayerState.h"

void UAmmoWidget::NativeConstruct()
{
	Super::NativeConstruct();

	SetVisibility(ESlateVisibility::Collapsed);
	if(ATestProjectCharacter* OwnerCharacter = Cast<ATestProjectCharacter>(GetOwningPlayerPawn()))
	{
		if(OwnerCharacter->GetWeapon())
		{
			GetOwningPlayerState<ATestProjectPlayerState>()->GetInventoryManager()->OnInventoryLoaded.AddWeakLambda(this,[this]
			{
				GetAmmoFromInventory(); //wait for inventory updated, if we restarting level
			});

			BindAmmoDelegate(OwnerCharacter->GetWeapon());
		}
		
		OwnerCharacter->OnWeaponUpdated.BindUObject(this, &ThisClass::BindAmmoDelegate);
	}
}

inline void UAmmoWidget::GetAmmoFromInventory()
{
	FInventoryItem FoundedSlot;
	GetOwningPlayerState<ATestProjectPlayerState>()->GetInventoryManager()->FindItem(Weapon->GetWeaponStats().AmmoClass,FoundedSlot);
	UpdateInventoryAmmo(0,FoundedSlot);
}

void UAmmoWidget::BindAmmoDelegate(AGeneralWeapon* NewWeapon)
{
	if(NewWeapon)
	{
		SetVisibility(ESlateVisibility::HitTestInvisible);
		Weapon = NewWeapon;
		
		UpdateAmmo(NewWeapon->GetAmmo());
		GetAmmoFromInventory();
		
		GetOwningPlayerState<ATestProjectPlayerState>()->GetInventoryManager()->OnInventoryChanged.AddUniqueDynamic(this,&ThisClass::UpdateInventoryAmmo);
		
		if(!NewWeapon->OnAmmoUpdated.IsBoundToObject(this))
		{
			NewWeapon->OnAmmoUpdated.AddUObject(this, &ThisClass::UpdateAmmo);
		}
	}
	else
	{
		SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UAmmoWidget::UpdateAmmo(const int32 Ammo) const
{
	AmmoText->SetText(FText::AsNumber(Ammo));
}

void UAmmoWidget::UpdateInventoryAmmo(int32 UpdatedSlotIndex, FInventoryItem NewItem)
{
	if(!Weapon || !NewItem.Item)
	{
		InventoryAmmoText->SetText(FText::AsNumber(0));
		return;
	}
	
	if(Weapon->GetWeaponStats().AmmoClass->GetName() == NewItem.Item->GetName())
	{
		const int32 AmmoCount = GetOwningPlayerState<ATestProjectPlayerState>()->GetInventoryManager()->GetItemCount(Weapon->GetWeaponStats().AmmoClass);
		InventoryAmmoText->SetText(FText::AsNumber(AmmoCount));
	}
}
