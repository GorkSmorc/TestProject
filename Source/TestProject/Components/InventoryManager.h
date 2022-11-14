// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/TreeView.h"
#include "Utilities/InventoryStruct.h"
#include "Utilities/WeaponInventoryStruct.h"
#include "InventoryManager.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnInventoryChanged,int32, ChangedSlotIndex, FInventoryItem, NewItem);
DECLARE_MULTICAST_DELEGATE(FOnInventoryLoaded)

UCLASS(meta=(BlueprintSpawnableComponent))
class TESTPROJECT_API UInventoryManager : public UActorComponent
{
	GENERATED_BODY()

public:

	UInventoryManager();

	UFUNCTION(BlueprintCallable)
	void LoadInventory();

	UFUNCTION(BlueprintCallable)
	void SaveInventory(bool bIsWin);
	
	UFUNCTION(BlueprintCallable, Category=Inventory)
	bool AddItem(TSubclassOf<APickableItem> ItemClass, int32 Count = 1);

	UFUNCTION(BlueprintCallable, Category=Inventory)
	bool AddWeapon(TSoftClassPtr<class AGeneralWeapon> ItemClass);

	UFUNCTION(BlueprintCallable, Category=Inventory)
	void RemoveItem(TSubclassOf<APickableItem> ItemClass, int32 Count = 1);

	UFUNCTION(BlueprintCallable, Category=Inventory)
	void ClearInventory();

	UPROPERTY(BlueprintAssignable, Category=Inventory)
	FOnInventoryChanged OnInventoryChanged;

	FOnInventoryLoaded OnInventoryLoaded;
	
	//Setters And Getters
	
	UFUNCTION(BlueprintCallable, Category=Inventory)
	FWeaponInventoryStruct GetNextWeapons(class AGeneralWeapon* CurrentWeapon);

	UFUNCTION(BlueprintCallable, Category=Inventory)
	FWeaponInventoryStruct GetPreviousWeapons(AGeneralWeapon* CurrentWeapon);

	UFUNCTION(BlueprintCallable)
	int32 GetMaxItems() const { return MaxItems; }
	
	UFUNCTION(BlueprintCallable, Category=Inventory)
	TArray<FInventoryItem> GetInventory() const { return Inventory; }
		
	UFUNCTION(BlueprintCallable, Category=Inventory)
	TArray<FWeaponInventoryStruct> GetWeaponInventory() const { return Weapons; }
	
	UFUNCTION(BlueprintCallable, Category=Inventory)
	bool FindItem(TSubclassOf<ADefaultItem> ItemClass, FInventoryItem& FoundedSlot);

	UFUNCTION(BlueprintCallable, Category=Inventory)
	int32 GetItemCount(TSubclassOf<ADefaultItem> ItemClass);

	bool bIsInventoryLoaded = false;

protected:

	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly)
	int32 MaxItems = 25;

	UPROPERTY(BlueprintReadOnly,Category=Inventory)
	TArray<FInventoryItem> Inventory;

	UPROPERTY(BlueprintReadOnly,Category=Inventory)
	TArray<FWeaponInventoryStruct> Weapons;

	UPROPERTY()
	class UProgressSave* Save = nullptr;
};
