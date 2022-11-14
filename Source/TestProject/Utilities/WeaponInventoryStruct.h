// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameObjects/Items/Weapons/GeneralWeapon.h"
#include "WeaponInventoryStruct.generated.h"

/**
 * 
 */

USTRUCT(BlueprintType)
struct FWeaponInventoryStruct
{
	GENERATED_BODY()
	
	UPROPERTY(BlueprintReadOnly,EditDefaultsOnly)
	TSoftClassPtr<AGeneralWeapon> Weapon;

	UPROPERTY(BlueprintReadOnly,EditDefaultsOnly)
	int32 Ammo;
		
	FWeaponInventoryStruct() :
		Weapon(nullptr),
		Ammo(10)
	{}

	FWeaponInventoryStruct(TSoftClassPtr<AGeneralWeapon> _Weapon, int32 _Ammo)
	{
		Ammo = _Ammo;
		Weapon = _Weapon;
	}

	FORCEINLINE bool operator==(const FWeaponInventoryStruct& Other) const
	{
		return Weapon == Other.Weapon && Ammo == Other.Ammo; 
	}

	FORCEINLINE bool operator==(const TSoftClassPtr<AGeneralWeapon>& Other) const
	{
		return Weapon == Other; 
	}
	
};

