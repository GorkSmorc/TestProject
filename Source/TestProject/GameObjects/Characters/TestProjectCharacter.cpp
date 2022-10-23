// Copyright Epic Games, Inc. All Rights Reserved.

#include "TestProjectCharacter.h"

#include "Animation/AnimInstance.h"
#include "Components/CapsuleComponent.h"
#include "Components/InventoryManager.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameObjects/Items/Weapons/GeneralWeapon.h"
#include "Kismet/GameplayStatics.h"
#include "MainClasses/TestProjectPlayerState.h"

ATestProjectCharacter::ATestProjectCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;
}

void ATestProjectCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void ATestProjectCharacter::EquipWeapon(TSubclassOf<AGeneralWeapon> NewWeaponClass, int32 Ammo)
{
	if(!NewWeaponClass)
	{
		if(Weapon)
		{
			Weapon->OnWeaponFired.Unbind();
			Weapon->OnWeaponReloading.Unbind();
			Weapon = nullptr;
		}
	
		return;
	}
	
	FActorSpawnParameters SpawnParameters = FActorSpawnParameters();
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	
	Weapon = GetWorld()->SpawnActor<AGeneralWeapon>(NewWeaponClass,GetActorTransform(), SpawnParameters);
	Weapon->SetAmmo(Ammo);
	Weapon->AttachToComponent(SkeletalMesh, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("SOCKET_Weapon"));
	SkeletalMesh->SetHiddenInGame(false);
		
	UAnimInstance* AnimInstance = SkeletalMesh->GetAnimInstance();
	if (AnimInstance != nullptr)
	{
		AnimInstance->Montage_Play(Weapon->GetWeaponStats().UnHolsterAnim, 1.f);
	}
		
	Weapon->OnWeaponFired.BindWeakLambda(this, [this]()
	{
		UAnimInstance* AnimInstance = SkeletalMesh->GetAnimInstance();
		if(AnimInstance == nullptr)
		{
			return;
		}

		if(Weapon->GetAmmo() > 0)
		{
			if (Weapon->GetWeaponStats().CharacterShootAnim != nullptr)
			{
				AnimInstance->Montage_Play(Weapon->GetWeaponStats().CharacterShootAnim, 1.f);
			}
		}
		else
		{
			if (Weapon->GetWeaponStats().CharacterShootEmptyAnim != nullptr)
			{
				AnimInstance->Montage_Play(Weapon->GetWeaponStats().CharacterShootEmptyAnim, 1.f);
			}
		}
	});

	Weapon->OnWeaponReloading.BindWeakLambda(this,[this](const bool bIsFull)
	{
		if(UAnimInstance* AnimInstance = SkeletalMesh->GetAnimInstance())
		{
			UAnimMontage* MontageToPlay = bIsFull ? Weapon->GetWeaponStats().CharacterReloadAnimFullEmpty : Weapon->GetWeaponStats().CharacterReloadAnim;
			AnimInstance->Montage_Play(MontageToPlay, 1.f);
		}
	});

	OnWeaponUpdated.ExecuteIfBound(Weapon);
}

void ATestProjectCharacter::OnFire()
{
	if(Weapon)
	{
		Weapon->OnFire();
	}
}

void ATestProjectCharacter::OnStopFire()
{
	if(Weapon)
	{
		Weapon->OnStopFire();
	}
}

void ATestProjectCharacter::Reloading()
{
	if(Weapon == nullptr)
	{
		return;
	}

	FInventoryItem FoundedSlot;
	UInventoryManager* InventoryManager = GetPlayerState<ATestProjectPlayerState>()->GetInventoryManager();
	if(InventoryManager->FindItem(Weapon->GetWeaponStats().AmmoClass, FoundedSlot))
	{
		int32 AmmoToLoad = FoundedSlot.Count;
		if(AmmoToLoad > Weapon->GetWeaponStats().MaxAmmo)
		{
			AmmoToLoad = Weapon->GetWeaponStats().MaxAmmo;
		}
		
		InventoryManager->RemoveItem(Weapon->GetWeaponStats().AmmoClass, AmmoToLoad);
		Weapon->OnWeaponReloading.BindWeakLambda(this,[this](const bool bIsFull)
		{
			if(UAnimInstance* AnimInstance = SkeletalMesh->GetAnimInstance())
			{
				UAnimMontage* MontageToPlay = bIsFull ? Weapon->GetWeaponStats().CharacterReloadAnimFullEmpty : Weapon->GetWeaponStats().CharacterReloadAnim;
				AnimInstance->Montage_Play(MontageToPlay, 1.f);
			}
		});

		Weapon->Reloading(FoundedSlot.Count);
	}
	
}

void ATestProjectCharacter::MoveForward(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorForwardVector(), Value);
	}
}

void ATestProjectCharacter::MoveRight(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorRightVector(), Value);
	}
}

void ATestProjectCharacter::Run()
{
	GetCharacterMovement()->MaxWalkSpeed = GetCharacterMovement()->MaxWalkSpeed * 1.5f;
}

void ATestProjectCharacter::StopRunning()
{
	GetCharacterMovement()->MaxWalkSpeed = GetCharacterMovement()->MaxWalkSpeed / 1.5f;
}
