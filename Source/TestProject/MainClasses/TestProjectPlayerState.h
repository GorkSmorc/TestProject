// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "TestProjectPlayerState.generated.h"

/**
 * 
 */

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnHealthChanged, int32 NewCurrentHealth, int32 NewMaxHealth);

UCLASS()
class TESTPROJECT_API ATestProjectPlayerState : public APlayerState
{
	GENERATED_BODY()

	ATestProjectPlayerState();

public:

	UFUNCTION(BlueprintCallable)
	class UInventoryManager* GetInventoryManager() const {return InventoryManager;}

	UFUNCTION(BlueprintCallable, Category=Health)
	void SetHealth(int32 NewHealth); 

	UFUNCTION(BlueprintCallable, Category=Health)
	int32 GetHealth() const {return CurrentHealth; }

	UFUNCTION(BlueprintCallable, Category=Health)
	void SetMaxHealth(int32 NewMaxHealth); 
	
	UFUNCTION(BlueprintCallable, Category=Health)
	int32 GetMaxHealth() const {return MaxHealth; }
	
	FOnHealthChanged OnHealthChanged;
	
protected:
	
	virtual void BeginPlay() override;

	UFUNCTION()
	void ResetLevelProgress();

	UPROPERTY(BlueprintReadOnly)
	UInventoryManager* InventoryManager;

	UPROPERTY(EditDefaultsOnly, Category=Health)
	int32 CurrentHealth = 100;

	UPROPERTY(EditDefaultsOnly, Category=Health)
	int32 MaxHealth = 100;
	
};

