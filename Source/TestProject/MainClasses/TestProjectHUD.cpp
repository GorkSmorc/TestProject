// Copyright Epic Games, Inc. All Rights Reserved.

#include "TestProjectHUD.h"
#include "CanvasItem.h"
#include "TestProjectGameMode.h"
#include "Blueprint/UserWidget.h"
#include "UI/FinishGameScreen.h"
#include "UI/Inventory/Inventory.h"
#include "UI/PlayerHUD.h"


ATestProjectHUD::ATestProjectHUD()
{
}

void ATestProjectHUD::BeginPlay()
{
	Super::BeginPlay();
	
	CreatePlayerHUD();
}

void ATestProjectHUD::CreatePlayerHUD()
{
	if(PlayerHUDClass != nullptr)
	{
		PlayerHUD = CreateWidget<UPlayerHUD>(PlayerOwner,PlayerHUDClass);
		PlayerHUD->AddToViewport();
	}

	PlayerOwner->GetPawn()->OnDestroyed.AddUniqueDynamic(this, &ThisClass::DestroyHUD);
	if(ATestProjectGameMode* GameMode = GetWorld()->GetAuthGameMode<ATestProjectGameMode>())
	{
		if(!GameMode->OnGameEnded.IsBoundToObject(this))
			GameMode->OnGameEnded.AddUObject(this, &ThisClass::ShowFinishScreen);

		if(!GameMode->OnGameRestarted.IsBoundToObject(this))
			GameMode->OnGameRestarted.AddUObject(this, &ThisClass::CreatePlayerHUD);
	}
}

void ATestProjectHUD::ShowHideInventory()
{
	if(InventoryClass != nullptr)
	{
		if(!Inventory)
		{
			Inventory = CreateWidget<UInventory>(PlayerOwner,InventoryClass);
		}

		if(!Inventory->IsInViewport())
		{
			Inventory->AddToViewport();
		}
		else
		{
			Inventory->RemoveFromParent();
		}
	}
}

void ATestProjectHUD::ShowFinishScreen(bool bIsWin) const
{
	if(!FinishWindowClass)
	{
		return;
	}
	
	UFinishGameScreen* FinishScreen = CreateWidget<UFinishGameScreen>(PlayerOwner, FinishWindowClass);
	FinishScreen->Init(bIsWin);
	FinishScreen->AddToViewport(10);

	GetOwningPlayerController()->SetShowMouseCursor(true);
}

void ATestProjectHUD::DestroyHUD(AActor* DestroyedActor)
{
	PlayerHUD->RemoveFromParent();
	PlayerHUD = nullptr;

	if(Inventory && Inventory->IsInViewport())
	{
		Inventory->RemoveFromParent();
	}

	Inventory = nullptr;
}
