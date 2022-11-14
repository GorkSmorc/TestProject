// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "Utilities/InventoryStruct.h"
#include "Utilities/WeaponInventoryStruct.h"
#include "ProgressSave.generated.h"

/**
 * 
 */
UCLASS()
class TESTPROJECT_API UProgressSave : public USaveGame
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable)
	void SetInventory(const TArray<FInventoryItem> InventoryToSave) { Inventory = InventoryToSave; };

	UFUNCTION(BlueprintCallable)
	TArray<FInventoryItem> GetInventory() const { return Inventory; };

	UFUNCTION(BlueprintCallable)
	void SetWeaponInventory(const TArray<FWeaponInventoryStruct> WeaponInventoryToSave) { WeaponInventory = WeaponInventoryToSave; };

	UFUNCTION(BlueprintCallable)
	TArray<FWeaponInventoryStruct> GetWeaponInventory() const { return WeaponInventory; };

	UFUNCTION(BlueprintCallable)
	void SetLastWonLevel(const FString LevelName) { LastWonLevel = LevelName; };

	UFUNCTION(BlueprintCallable)
	FString GetLastWonLevel() const { return LastWonLevel; };

private:

	UPROPERTY()
	TArray<FInventoryItem> Inventory;

	UPROPERTY()
	TArray<FWeaponInventoryStruct> WeaponInventory;

	UPROPERTY()
	FString LastWonLevel = "";
};
