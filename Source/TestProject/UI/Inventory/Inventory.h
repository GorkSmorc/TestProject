// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Utilites/InventoryStruct.h"
#include "Inventory.generated.h"

class UInventorySlot;
/**
 * 
 */
UCLASS()
class TESTPROJECT_API UInventory : public UUserWidget
{
	GENERATED_BODY()

protected:
	
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	UFUNCTION()
	void ShowContextWindow(UInventorySlot* ClickedSlot);
	
	UFUNCTION(BlueprintCallable)
	void GenerateSlots();

	UFUNCTION(BlueprintCallable)
	void UpdateSlots();
		
	UFUNCTION()
	void OnInventorySlotClicked(UInventorySlot* ClickedSlot);

	UFUNCTION()
	void OnInventoryUpdated(int32 ChangedSlotIndex, FInventoryItem NewItem);
	
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	class UWrapBox* WrapBox = nullptr;

	UPROPERTY(BlueprintReadOnly)
	TArray<UInventorySlot*> Slots;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UInventorySlot> SlotClass = nullptr;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UContextWindow> ContextWindowClass = nullptr;

private:

	UPROPERTY()
	UContextWindow* ContextWindow = nullptr;
	
};
