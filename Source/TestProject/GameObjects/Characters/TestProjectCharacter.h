// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TestProjectCharacter.generated.h"

class UInputComponent;
class USkeletalMeshComponent;
class USceneComponent;
class UCameraComponent;
class UMotionControllerComponent;
class UAnimMontage;
class USoundBase;
class AGeneralWeapon;

DECLARE_DELEGATE_OneParam(FOnWeaponUpdated, AGeneralWeapon* NewWeapon);

UCLASS(Abstract,config=Game)
class ATestProjectCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ATestProjectCharacter();

protected:
	
	virtual void BeginPlay() override;
	
public:
	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

	UFUNCTION(BlueprintCallable)
	AGeneralWeapon* GetWeapon() const { return Weapon; }

	UFUNCTION(BlueprintCallable)
	void EquipWeapon(TSubclassOf<AGeneralWeapon> NewWeaponClass, int32 Ammo);

protected:
	
	/** Fires a projectile. */
	UFUNCTION(BlueprintCallable)
	virtual void OnFire();

	UFUNCTION(BlueprintCallable)
	virtual void OnStopFire();

	UFUNCTION(BlueprintCallable)
	virtual void Reloading();
	
	void MoveForward(float Val);
	void MoveRight(float Val);
	void Run();
	void StopRunning();
		
	UPROPERTY(BlueprintReadOnly, Category=Equipment)
	AGeneralWeapon* Weapon = nullptr;

	UPROPERTY()
	USkeletalMeshComponent* SkeletalMesh = nullptr;

public:
	
	FOnWeaponUpdated OnWeaponUpdated;

};
