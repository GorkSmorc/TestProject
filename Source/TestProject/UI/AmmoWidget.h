// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Utilities/InventoryStruct.h"
#include "AmmoWidget.generated.h"

/**
 * 
 */
UCLASS()
class TESTPROJECT_API UAmmoWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	
	virtual void NativeConstruct() override;
		
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,meta=(BindWidget))
	class UTextBlock* AmmoText = nullptr;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,meta=(BindWidget))
	UTextBlock* InventoryAmmoText = nullptr; 

private:

	UFUNCTION()
	void GetAmmoFromInventory();
	
	UFUNCTION()
	void BindAmmoDelegate(class AGeneralWeapon* NewWeapon);

	UFUNCTION()
	void UpdateAmmo(int32 Ammo) const;

	UFUNCTION()
	void UpdateInventoryAmmo(int32 UpdatedSlotIndex, FInventoryItem NewItem);

	UPROPERTY()
	AGeneralWeapon* Weapon = nullptr;
};


