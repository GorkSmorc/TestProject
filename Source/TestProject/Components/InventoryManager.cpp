// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryManager.h"
#include "GameObjects/Items/Weapons/GeneralWeapon.h"
#include "Kismet/GameplayStatics.h"
#include "MainClasses/TestProjectGameMode.h"
#include "Saves/ProgressSave.h"


UInventoryManager::UInventoryManager()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UInventoryManager::LoadInventory()
{
	Save = Cast<UProgressSave>(UGameplayStatics::LoadGameFromSlot("Progress",1));
	if(Save)
	{
		Inventory = Save->GetInventory();
		Weapons = Save->GetWeaponInventory();
		OnInventoryLoaded.Broadcast();
		bIsInventoryLoaded = true;
	}
}

void UInventoryManager::SaveInventory(bool bIsWin)
{
	if(!bIsWin)
		return;

	if(!Save)
	{
		Save = Cast<UProgressSave>(UGameplayStatics::CreateSaveGameObject(UProgressSave::StaticClass()));
		if(!Save)
		{
			return;
		}
	}

	Save->SetInventory(Inventory);
	Save->SetWeaponInventory(Weapons);
	Save->SetLastWonLevel(GetWorld()->GetName());
	UGameplayStatics::SaveGameToSlot(Save,"Progress",1);
}

FWeaponInventoryStruct UInventoryManager::GetNextWeapons(AGeneralWeapon* CurrentWeapon)
{
	for(size_t i = 0; i < Weapons.Num(); i++)
	{
		if(CurrentWeapon->GetClass()->GetName() == Weapons[i].Weapon->GetName())
		{
			if(Weapons.IsValidIndex(i+1))
			{
				Weapons[i].Ammo = CurrentWeapon->GetAmmo();
				return Weapons[i+1];
			}
			else if(i != 0)
			{
				Weapons[i].Ammo = CurrentWeapon->GetAmmo();
				return  Weapons[0];
			}
			else
			{
				return FWeaponInventoryStruct(nullptr,0);
			}
		}
	}

	return FWeaponInventoryStruct(nullptr,0);
}

FWeaponInventoryStruct UInventoryManager::GetPreviousWeapons(AGeneralWeapon* CurrentWeapon)
{
	for(size_t i = 0; i < Weapons.Num(); i++)
	{
		if(CurrentWeapon->GetClass()->GetName() == Weapons[i].Weapon->GetName())
		{
			if(Inventory.IsValidIndex(i-1))
			{
				Weapons[i].Ammo = CurrentWeapon->GetAmmo();
				return Weapons[i-1];
			}
			else if(i != Weapons.Num()-1)
			{
				Weapons[i].Ammo = CurrentWeapon->GetAmmo();
				return  Weapons.Last();
			}
			else
			{
				return FWeaponInventoryStruct(nullptr,0);
			}
		}
	}

	return FWeaponInventoryStruct(nullptr,0);
}

bool UInventoryManager::FindItem(const TSubclassOf<ADefaultItem> ItemClass, FInventoryItem& FoundedSlot)
{
	for(const FInventoryItem Slot : Inventory)
	{
		if(Slot.Item->GetName() == ItemClass->GetName())
		{
			FoundedSlot = Slot;
			return true;
		}
	}

	return false;
}

int32 UInventoryManager::GetItemCount(const TSubclassOf<ADefaultItem> ItemClass)
{
	int32 Count = 0;
	for(const FInventoryItem Item : Inventory)
	{
		if(Item.Item->GetName() == ItemClass->GetName())
		{
			Count += Item.Count;
		}
	}

	return Count;
}

void UInventoryManager::BeginPlay()
{
	Super::BeginPlay();

	if(ATestProjectGameMode* GameMode = GetWorld()->GetAuthGameMode<ATestProjectGameMode>())
	{
		GameMode->OnGameEnded.AddUObject(this, &ThisClass::SaveInventory);
		GameMode->OnGameRestarted.AddUniqueDynamic(this, &ThisClass::LoadInventory);
	}

	LoadInventory();
}

bool UInventoryManager::AddItem(const TSubclassOf<APickableItem> ItemClass, const int32 Count)
{
	if(Inventory.Num() >= MaxItems)
	{
		return false;
	}
	
	size_t ItemIndex = -1;
	
	for(size_t i = 0; i < Inventory.Num(); i++)
	{
		if(Inventory.IsValidIndex(i))
		{
			if(Inventory[i].Item->GetName() == ItemClass->GetName())
			{
				ItemIndex = i;
				
				if(Inventory[i].Count != Inventory[i].StackSize)
				{
					break;
				}
			}
		}
	}

	if(ItemIndex == -1)
	{
		Inventory.Add(FInventoryItem(ItemClass,Count));
		OnInventoryChanged.Broadcast(Inventory.Num()-1, Inventory.Last());
		return true;
	}

	if(Inventory[ItemIndex].Count == Inventory[ItemIndex].StackSize)
	{
		return false;
	}
		
	if(Inventory[ItemIndex].Count + Count > Inventory[ItemIndex].StackSize)
	{
		if(Inventory.Num() + 1 >= MaxItems)
		{
			Inventory[ItemIndex] = FInventoryItem(ItemClass,Inventory[ItemIndex].StackSize);
		}
		else
		{
			Inventory.Add(FInventoryItem(Inventory[ItemIndex].Item.Get(), Inventory[ItemIndex].Count + Count -Inventory[ItemIndex].StackSize));
			Inventory[ItemIndex] = FInventoryItem(ItemClass,Inventory[ItemIndex].StackSize);
		}
	}
	else
	{
		Inventory[ItemIndex] = FInventoryItem(ItemClass, Inventory[ItemIndex].Count + Count);
	}

	OnInventoryChanged.Broadcast(ItemIndex, Inventory[ItemIndex]);
	return true;
}

bool UInventoryManager::AddWeapon(TSoftClassPtr<AGeneralWeapon> ItemClass)
{
	const TSubclassOf<AGeneralWeapon> WeaponClass = ItemClass.LoadSynchronous();
	const AGeneralWeapon* DefaultWeapon = WeaponClass->GetDefaultObject<AGeneralWeapon>();
	
	if(Weapons.Contains(ItemClass))
	{
		return AddItem(DefaultWeapon->GetWeaponStats().AmmoClass,DefaultWeapon->GetWeaponStats().MaxAmmo);
	}
	
	Weapons.Add(FWeaponInventoryStruct(ItemClass,DefaultWeapon->GetWeaponStats().MaxAmmo));
	return true;
}

void UInventoryManager::RemoveItem(const TSubclassOf<APickableItem> ItemClass, const int32 Count)
{
	size_t ItemIndex = -1;
	
	for(size_t i = Inventory.Num()-1; i != -1; i--)
	{
		if(Inventory[i].Item->GetName() == ItemClass->GetName())
		{
			ItemIndex = i;
			break;
		}
	}

	if(ItemIndex == -1)
	{
		return;
	}

	if(Inventory[ItemIndex].Count - Count < 1)
	{
		Inventory.RemoveAt(ItemIndex);
		OnInventoryChanged.Broadcast(ItemIndex, FInventoryItem(ItemClass,0));
	}
	else
	{
		Inventory[ItemIndex] = FInventoryItem(ItemClass, Inventory[ItemIndex].Count - Count);
		OnInventoryChanged.Broadcast(ItemIndex, Inventory[ItemIndex]);
	}
}

void UInventoryManager::ClearInventory()
{
	Inventory.Empty();
	Weapons.Empty();
}
