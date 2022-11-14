// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameObjects/Items/PickableItem.h"
#include "InventoryStruct.generated.h"

/**
 * 
 */

USTRUCT(BlueprintType)
struct FInventoryItem
{
	GENERATED_BODY()
	
	UPROPERTY(BlueprintReadOnly,EditDefaultsOnly)
	TSoftClassPtr<APickableItem> Item;

	UPROPERTY(BlueprintReadOnly,EditDefaultsOnly)
	int32 Count;

	UPROPERTY(BlueprintReadOnly,EditDefaultsOnly)
	int32 StackSize;
	
	FInventoryItem() :
		Item(nullptr),
		Count(10),
		StackSize(255)
	{}

	FInventoryItem(TSubclassOf<APickableItem> ItemClass, int32 ItemCount)
	{
		Item = ItemClass;
		Count = ItemCount;
		StackSize = ItemClass->GetDefaultObject<APickableItem>()->StackSize;
	}
	
};

