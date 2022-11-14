// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameObjects/Items/DefaultItem.h"
#include "Utilities/WeaponStats.h"
#include "GeneralWeapon.generated.h"

DECLARE_DELEGATE(FOnWeaponFired)
DECLARE_DELEGATE_OneParam(FOnWeaponReloading,bool bIsFull);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnAmmoUpdated, int32 Ammo)
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWeaponReloaded, int32, AmmoLoaded);

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	Pistol			UMETA(DisplayName="Pistol"),
	AssaultRifle	UMETA(DisplayName="AssaultRifle")
};

UCLASS(Abstract)
class TESTPROJECT_API AGeneralWeapon : public ADefaultItem
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGeneralWeapon();

	UFUNCTION(BlueprintCallable)
	void OnFire();

	UFUNCTION(BlueprintCallable)
	void Reloading(int32 AmmoToLoad);
	
	UFUNCTION(BlueprintCallable)
	virtual void OnStopFire();
	
	UFUNCTION(BlueprintCallable)
	USkeletalMeshComponent* GetMesh() const { return FP_Gun; }

	UFUNCTION(BlueprintCallable)
	FWeaponStats GetWeaponStats() const { return WeaponStats; }

	UFUNCTION(BlueprintCallable)
	EWeaponType GetWeaponType() const { return WeaponType; }

	UFUNCTION(BlueprintCallable)
	void SetAmmo(int32 NewAmmo) { Ammo = NewAmmo; }
	
	UFUNCTION(BlueprintCallable)
	int32 GetAmmo() const { return Ammo; }

	UFUNCTION(BlueprintCallable)
	void SetMaxDispersion(float NewDispersion) { WeaponStats.MaxDispersion = NewDispersion; };

	bool bReloading = false;

	UFUNCTION(BlueprintCallable, Category=Cheats)
	void EnableUnlimitedAmmo() { bIsUnlimitedAmmoEnabled = true; };

	UFUNCTION(BlueprintCallable, Category=Cheats)
	void DisableUnlimitedAmmo() { bIsUnlimitedAmmoEnabled = false; };

protected:

	virtual void Tick(float DeltaSeconds) override;
	virtual void BeginPlay() override;
	
	UFUNCTION(BlueprintImplementableEvent)
	void CreateHitDecal(FHitResult Hit);
	
	UFUNCTION()
	virtual void CreateTrace();
		
	UPROPERTY(EditDefaultsOnly, Category = Mesh)
	USkeletalMeshComponent* FP_Gun = nullptr;
	
	UPROPERTY(EditDefaultsOnly, Category = Mesh)
	UParticleSystemComponent* Particle = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Gameplay)
	FWeaponStats WeaponStats;
	
	UPROPERTY(EditAnywhere)
	EWeaponType WeaponType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UArrowComponent* Arrow = nullptr;

	UPROPERTY(EditAnywhere)
	float BulletForce = 200000.f;
	
public:

	FOnWeaponFired OnWeaponFired;
	FOnWeaponReloading OnWeaponReloading;
	FOnAmmoUpdated OnAmmoUpdated;
	
	UPROPERTY(BlueprintAssignable)
	FOnWeaponReloaded OnWeaponReloaded;
	
private:

	UFUNCTION()
	void OnReloadEnded(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload);

	FTimerHandle ShootTimer;
	
	int32 Ammo = 30;
	
	bool bCooldown = false;
	bool EmptySoundCooldown = false;

	int32 AmmoToLoad;
	
	UPROPERTY(BlueprintReadOnly, meta=(AllowPrivateAccess))
	float Dispersion = 0;
	float DispersionReduce = WeaponStats.DispersionPerShot;

	bool bIsUnlimitedAmmoEnabled = false;
};
