// Fill out your copyright notice in the Description page of Project Settings.


#include "MainClasses/TestProjectPlayerState.h"

#include "TestProjectGameMode.h"
#include "Components/InventoryManager.h"
#include "Kismet/GameplayStatics.h"

ATestProjectPlayerState::ATestProjectPlayerState() : Super()
{
	InventoryManager = CreateDefaultSubobject<UInventoryManager>("InventoryManager");
}

void ATestProjectPlayerState::SetHealth(int32 NewHealth)
{
	CurrentHealth = NewHealth;
	OnHealthChanged.Broadcast(CurrentHealth, MaxHealth);
}

void ATestProjectPlayerState::SetMaxHealth(int32 NewMaxHealth)
{
	MaxHealth = NewMaxHealth;
	OnHealthChanged.Broadcast(CurrentHealth,MaxHealth);
}

void ATestProjectPlayerState::BeginPlay()
{
	Super::BeginPlay();

	if(ATestProjectGameMode* GameMode = GetWorld()->GetAuthGameMode<ATestProjectGameMode>())
	{
		GameMode->OnGameRestarted.AddUObject(this, &ThisClass::ResetLevelProgress);
	}	
}

void ATestProjectPlayerState::ResetLevelProgress()
{
	SetHealth(MaxHealth);
}
