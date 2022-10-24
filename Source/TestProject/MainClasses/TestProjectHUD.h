// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once 

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "TestProjectHUD.generated.h"

class UInventory;
class UPlayerHUD;

UCLASS()
class ATestProjectHUD : public AHUD
{
	GENERATED_BODY()
	
public:
	ATestProjectHUD();
	
	virtual void BeginPlay() override;

	UFUNCTION()
	void CreatePlayerHUD();
	
	UPlayerHUD* GetPlayerHUD() const { return PlayerHUD; }
	UInventory* GetInventory() const { return Inventory; }

	UFUNCTION(BlueprintCallable)
	void ShowHideInventory();

	UFUNCTION(BlueprintImplementableEvent)
	void ShowHideHelp();

protected:

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void CreateRequestToExitWidget();

	UFUNCTION()
	void ShowFinishScreen(bool bIsWin) const;

	UFUNCTION()
	void DestroyHUD(AActor* DestroyedActor);

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> PlayerHUDClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> InventoryClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> FinishWindowClass;

	UPROPERTY(BlueprintReadOnly)
	UPlayerHUD* PlayerHUD = nullptr;

	UPROPERTY(BlueprintReadOnly)
	UInventory* Inventory = nullptr;
	
};

