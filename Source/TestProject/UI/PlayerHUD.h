// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerHUD.generated.h"

class UInventory;
/**
 * 
 */
class UHealthBar;
class UAmmoWidget;

UCLASS()
class TESTPROJECT_API UPlayerHUD : public UUserWidget
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable)
	UHealthBar* GetHealthBar() const { return HealthBar; }

	UFUNCTION(BlueprintImplementableEvent)
	void PlayGetHitAnim();

	UFUNCTION(BlueprintCallable)
	UAmmoWidget* GetAmmoWidget() const { return Ammo; }
	
	UFUNCTION(BlueprintCallable)
	UInventory* GetInventoryWidget() const { return Inventory; }
	
	UFUNCTION(BlueprintCallable)
	bool ToggleInventory();
	
	UFUNCTION(BlueprintCallable)
	bool ToggleHelp();

protected:

	virtual void NativeConstruct() override;
	
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,meta=(BindWidget))
	TObjectPtr<UHealthBar> HealthBar = nullptr;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,meta=(BindWidget))
	TObjectPtr<UAmmoWidget> Ammo = nullptr;
	
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,meta=(BindWidget))
	TObjectPtr<UInventory> Inventory = nullptr;
	
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,meta=(BindWidget))
	TObjectPtr<UUserWidget> Help = nullptr;
};
