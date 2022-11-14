// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WeaponStats.generated.h"

/**
 * 
 */

USTRUCT(BlueprintType)
struct FWeaponStats
{
	GENERATED_BODY()
	
	//Shoots per second
	UPROPERTY(BlueprintReadOnly,EditDefaultsOnly)
	float FireRate;

	UPROPERTY(BlueprintReadOnly,EditDefaultsOnly)
	float DispersionPerShot;

	UPROPERTY(BlueprintReadOnly,EditDefaultsOnly)
	float MaxDispersion;

	UPROPERTY(BlueprintReadOnly,EditDefaultsOnly)
	float DispersionReduceDivisor;

	UPROPERTY(BlueprintReadOnly,EditDefaultsOnly)
	TSubclassOf<class APickableItem> AmmoClass;
	
	UPROPERTY(BlueprintReadOnly,EditDefaultsOnly)
	int32 MaxAmmo;

	UPROPERTY(BlueprintReadOnly,EditDefaultsOnly)
	int32 Damage;

	UPROPERTY(BlueprintReadOnly,EditDefaultsOnly)
	USoundBase* FireSound;

	UPROPERTY(BlueprintReadOnly,EditDefaultsOnly)
	USoundBase* EmptySound;
	
	UPROPERTY(BlueprintReadOnly,EditDefaultsOnly)
	UAnimMontage* WeaponShootAnim;
	
	UPROPERTY(BlueprintReadOnly,EditDefaultsOnly)
	UAnimMontage* CharacterShootAnim;

	UPROPERTY(BlueprintReadOnly,EditDefaultsOnly)
	UAnimMontage* CharacterShootEmptyAnim;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	UAnimMontage* WeaponReloadAnimFullEmpty;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	UAnimMontage* CharacterReloadAnimFullEmpty;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	UAnimMontage* WeaponReloadAnim;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	UAnimMontage* CharacterReloadAnim;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	UAnimMontage* HolsterAnim;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	UAnimMontage* UnHolsterAnim;

	FWeaponStats() :
		FireRate(0.25f),
		DispersionPerShot(1.f),
		MaxDispersion(10.f),
		DispersionReduceDivisor(2.f),
		AmmoClass(nullptr),
		MaxAmmo(30),
		Damage(10),
		FireSound(nullptr),
		EmptySound(nullptr),
		WeaponShootAnim(nullptr),
		CharacterShootAnim(nullptr),
		CharacterShootEmptyAnim(nullptr),
		WeaponReloadAnimFullEmpty(nullptr),
		WeaponReloadAnim(nullptr),
		HolsterAnim(nullptr),
		UnHolsterAnim(nullptr)
	{}
	
};

