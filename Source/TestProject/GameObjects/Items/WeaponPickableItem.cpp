// Fill out your copyright notice in the Description page of Project Settings.


#include "GameObjects/Items/WeaponPickableItem.h"

#include <Kismet/GameplayStatics.h>

#include "Components/InventoryManager.h"
#include "GameObjects/Items/Weapons/GeneralWeapon.h"
#include "GameObjects/Characters/TestProjectCharacter.h"
#include "MainClasses/TestProjectPlayerState.h"

void AWeaponPickableItem::OnPickedUp(ATestProjectCharacter* OverlappedCharacter)
{
	if(OverlappedCharacter->IsPlayerControlled())
	{
		if(OverlappedCharacter->GetPlayerState<ATestProjectPlayerState>()->GetInventoryManager()->AddWeapon(ItemClass))
		{
			UGameplayStatics::PlaySound2D(this, Sound, 0.7f);
			Destroy();
		}
	}
	
	if(OverlappedCharacter->GetWeapon() == nullptr)
	{
		const TSubclassOf<AGeneralWeapon> WeaponClass = ItemClass.LoadSynchronous();
		OverlappedCharacter->EquipWeapon(WeaponClass,WeaponClass->GetDefaultObject<AGeneralWeapon>()->GetWeaponStats().MaxAmmo);
	}
}
