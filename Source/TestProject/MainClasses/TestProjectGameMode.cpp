// Copyright Epic Games, Inc. All Rights Reserved.

#include "TestProjectGameMode.h"
#include "TestProjectHUD.h"
#include "Components/InventoryManager.h"
#include "Engine/TargetPoint.h"
#include "GameFramework/PlayerStart.h"
#include "GameObjects/Characters/AICharacter.h"
#include "GameObjects/Characters/PlayerCharacter.h"
#include "GameObjects/Characters/TestProjectCharacter.h"
#include "GameObjects/Items/ItemSpawner.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"

ATestProjectGameMode::ATestProjectGameMode() : Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPersonCPP/Blueprints/BP_Player"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = ATestProjectHUD::StaticClass();
}

void ATestProjectGameMode::StartGame()
{
	if(!BotClass)
	{
		UE_LOG(LogTemp,Error,TEXT("Bot class didn't setted in gamemode"))
		return;
	}
	
	TArray<AActor*> BotSpawnPoints;
	UGameplayStatics::GetAllActorsOfClass(this,ATargetPoint::StaticClass(),BotSpawnPoints);

	for(const AActor* Point : BotSpawnPoints)
	{
		FActorSpawnParameters ActorSpawnParameters;
		ActorSpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;
		AAICharacter* Bot = GetWorld()->SpawnActor<AAICharacter>(BotClass,Point->GetActorTransform(),ActorSpawnParameters);
		if(Bot)
		{
			Bot->OnBotDead.BindUObject(this, &ThisClass::OnBotRipped);
			Bots.Add(Bot);
		}
		else
		{
			UE_LOG(LogTemp,Error,TEXT("Bot at point %s cannot be spawn!"), *Point->GetName())
		}
	}
	
	OnBotCountChanged.Broadcast(Bots.Num());
	
}

void ATestProjectGameMode::OnBotRipped(AActor* Bot)
{
	BotKilled++;
	
	OnBotCountChanged.Broadcast(Bots.Num() - BotKilled);
	
	if(Bots.Num() == BotKilled)
	{
		EndGame(true);
	}
}

void ATestProjectGameMode::EndGame(bool bIsWin)
{
	OnGameEnded.Broadcast(bIsWin);
}

void ATestProjectGameMode::OnPlayerSpawned(APlayerCharacter* Character)
{
	Character->OnPlayerDead.BindUObject(this, &ThisClass::OnEndGame);
	StartGame();
}

void ATestProjectGameMode::ResetGame()
{
	//Kill lived bots and bot's bodyes
	for(AActor* Bot : Bots)
	{
		Bot->Destroy();
	}

	Bots.Empty();
	BotKilled = 0;

	//Return time scale
	UGameplayStatics::SetGlobalTimeDilation(this,1.f);
	UGameplayStatics::SetGamePaused(this,false);

	//Spawn new Player
	const AActor* PlayerStart = UGameplayStatics::GetActorOfClass(this,APlayerStart::StaticClass());
	APlayerCharacter* NewCharacter = GetWorld()->SpawnActor<APlayerCharacter>(DefaultPawnClass,PlayerStart->GetActorTransform(),FActorSpawnParameters());

	//Destroy SpectatorPawn
	APlayerController* PlayerController = GetGameInstance()->GetFirstLocalPlayerController(GetWorld());
	if(APawn* Pawn = PlayerController->GetPawnOrSpectator())
	{
		Pawn->Destroy();
	}

	//Reset all spawners
	TArray<AActor*> ItemSpawners;
	UGameplayStatics::GetAllActorsOfClass(this,AItemSpawner::StaticClass(),ItemSpawners);

	for(AActor* Spawner : ItemSpawners)
	{
		if(AItemSpawner* CastedSpawner = Cast<AItemSpawner>(Spawner))
		{
			CastedSpawner->ResetSpawner();
		}
	}
	
	PlayerController->Possess(NewCharacter);
	PlayerController->EndViewTarget(PlayerController);
	PlayerController->FlushPressedKeys();

	OnGameRestarted.Broadcast();
}
