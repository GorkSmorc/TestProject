// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameObjects/Characters/TestProjectCharacter.h"
#include "Utilites/WeaponInventoryStruct.h"
#include "PlayerCharacter.generated.h"

/**
 * 
 */

DECLARE_DELEGATE(FOnPlayerDead);
DECLARE_DELEGATE(FOnPlayerTookDamage);
DECLARE_DELEGATE(FRequestToExit);

UCLASS()
class TESTPROJECT_API APlayerCharacter : public ATestProjectCharacter
{
	GENERATED_BODY()

	APlayerCharacter();

public:
	
	/** Returns Mesh1P subobject **/
	UFUNCTION(BlueprintCallable)
	USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; }
	
	/** Returns FirstPersonCameraComponent subobject **/
	UFUNCTION(BlueprintCallable)
	UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }

	FOnPlayerDead OnPlayerDead;
	FOnPlayerTookDamage OnPlayerTookDamage;
	FRequestToExit RequestToExit;

protected:

	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
	virtual void AddControllerYawInput(float Val) override;
	virtual void AddControllerPitchInput(float Val) override;;
	virtual void PossessedBy(AController* NewController) override;
	virtual void Destroyed() override;

	virtual void OnFire() override;
	virtual void Reloading() override;
	
	virtual float InternalTakePointDamage(float Damage, FPointDamageEvent const& PointDamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	UFUNCTION()
	void OnInventoryClicked();

	UFUNCTION()
	void OnHelpClicked();
	
	UFUNCTION()
	void OnCrouchStarted();

	UFUNCTION()
	void OnCrouchEnded();
		
	UFUNCTION()
	void Death(AActor* Killer);

	UFUNCTION()
	void OnNextWeapon();

	UFUNCTION()
	void OnPreviousWeapon();

	UFUNCTION()
	void RequestToExitToMainMenu();

	UFUNCTION()
	void SwitchWeapon(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload);

	/** Pawn mesh: 1st person view (arms; seen only by self) */
	UPROPERTY(VisibleDefaultsOnly, Category=Mesh)
	USkeletalMeshComponent* Mesh1P;

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FirstPersonCameraComponent;

private:
	
	void OnInteractClicked();
	void TurnAtRate(float Rate);
	void LookUpAtRate(float Rate);
	

	UFUNCTION()
	void RemoveAmmoFromInventory(int32 AmmoLoaded);

	bool bInWidget = false;
	bool bSwitchingWeapon = false;
	FWeaponInventoryStruct NewWeapon;

};


