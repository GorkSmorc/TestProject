// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Inventory/Inventory.h"
#include "Utilities/InventoryStruct.h"
#include "InventorySlot.generated.h"

/**
 * 
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSlotClicked, UInventorySlot*, ClickedSlot);

UCLASS()
class TESTPROJECT_API UInventorySlot : public UUserWidget
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable)
	void InitSlot(FInventoryItem Item);

	UFUNCTION(BlueprintCallable)
	void ClearSlot();
	
	UFUNCTION(BlueprintCallable)
	FInventoryItem GetItem() const { return Item; }

	UPROPERTY(BlueprintAssignable)
	FOnSlotClicked OnSlotClicked;
	
protected:
	
	virtual void NativeConstruct() override;
		
	UFUNCTION()
	void OnSlotButtonClicked();
		
	UPROPERTY(BlueprintReadOnly,meta=(BindWidget))
	class UButton* SlotButton = nullptr;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	class UImage* ItemImage = nullptr;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	class UTextBlock* CountText = nullptr;

	UPROPERTY(BlueprintReadOnly)
	FInventoryItem Item;
	
};
