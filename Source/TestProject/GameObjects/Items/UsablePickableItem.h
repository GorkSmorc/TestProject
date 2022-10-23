// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameObjects/Items/PickableItem.h"
#include "Utilites/UseInterface.h"
#include "UsablePickableItem.generated.h"

/**
 * 
 */
UCLASS()
class TESTPROJECT_API AUsablePickableItem : public APickableItem, public IUseInterface
{
	GENERATED_BODY()

};
