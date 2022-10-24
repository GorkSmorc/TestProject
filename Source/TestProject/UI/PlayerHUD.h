// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerHUD.generated.h"

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
	
protected:

	virtual void NativeConstruct() override;
	
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,meta=(BindWidget))
	UHealthBar* HealthBar = nullptr;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,meta=(BindWidget))
	UAmmoWidget* Ammo = nullptr;
	
};
