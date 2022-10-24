// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TestProjectGameMode.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnGameEnded, bool bIsWin);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnBotKilled, int32 BotsLeft)
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGameRestarted);

UCLASS(minimalapi)
class ATestProjectGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ATestProjectGameMode();

	UFUNCTION(BlueprintCallable)
	void OnPlayerSpawned(class APlayerCharacter* Character);

	UFUNCTION(BlueprintCallable)
	int32 GetBotsLeft() const { return Bots.Num(); }

	UFUNCTION(BlueprintCallable)
	int32 GetKilledBotsCount() const { return BotKilled; }

	UFUNCTION(BlueprintCallable)
	void ResetGame();

	FOnGameEnded OnGameEnded;
	FOnBotKilled OnBotCountChanged;

	UPROPERTY(BlueprintAssignable)
	FOnGameRestarted OnGameRestarted;

protected:
	
	void OnBotRipped(AActor* Bot);

	UFUNCTION(BlueprintCallable)
	void EndGame(bool bIsWin);
	
	UFUNCTION(BlueprintCallable)
	void StartGame();
	
	UFUNCTION(BlueprintImplementableEvent)
	void OnEndGame();

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class AAICharacter> BotClass;

private:

	UPROPERTY()
	int32 BotKilled = 0;
	
	UPROPERTY()
	TArray<AActor*> Bots;
};



