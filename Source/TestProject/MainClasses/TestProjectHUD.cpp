// Copyright Epic Games, Inc. All Rights Reserved.

#include "TestProjectHUD.h"
#include "TestProjectGameMode.h"
#include "Blueprint/UserWidget.h"
#include "GameObjects/Characters/PlayerCharacter.h"
#include "UI/FinishGameScreen.h"
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

		if(!GameMode->OnGameRestarted.IsAlreadyBound(this, &ThisClass::CreatePlayerHUD))
			GameMode->OnGameRestarted.AddUniqueDynamic(this, &ThisClass::CreatePlayerHUD);
	}

	PlayerOwner->GetPawn<APlayerCharacter>()->RequestToExit.BindUObject(this, &ThisClass::CreateRequestToExitWidget);
	PlayerOwner->SetShowMouseCursor(false);
	PlayerOwner->SetInputMode(FInputModeGameOnly());
}

bool ATestProjectHUD::ToggleInventory() const
{
	check(PlayerHUD)
	return PlayerHUD->ToggleInventory();
}

bool ATestProjectHUD::ToggleHelp() const
{
	check(PlayerHUD)
	return PlayerHUD->ToggleHelp();
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
}
