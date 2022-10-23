// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameObjects/Items/PickableItem.h"
#include "WeaponPickableItem.generated.h"

/**
 * 
 */
UCLASS()
class TESTPROJECT_API AWeaponPickableItem : public APickableItem
{
	GENERATED_BODY()
	
	virtual void OnPickedUp(class ATestProjectCharacter* OverlappedCharacter) override;
	
public:
	
	UFUNCTION(BlueprintCallable)
	TSoftClassPtr<class AGeneralWeapon> GetItemClass() const { return ItemClass; };

protected:
	
	UPROPERTY(EditAnywhere)
	TSoftClassPtr<AGeneralWeapon> ItemClass = nullptr;
};
