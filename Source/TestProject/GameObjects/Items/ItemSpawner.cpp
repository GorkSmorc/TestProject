// Fill out your copyright notice in the Description page of Project Settings.


#include "GameObjects/Items/ItemSpawner.h"

#include "DefaultItem.h"

// Sets default values
AItemSpawner::AItemSpawner()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AItemSpawner::ResetSpawner()
{
	GetWorldTimerManager().ClearTimer(SpawnTimer);

	if(SpawnedActor)
	{
		SpawnedActor->Destroy();
		SpawnedActor = nullptr;
	}

	SpawnItem();
	
	GetWorldTimerManager().SetTimer(SpawnTimer,this,&ThisClass::SpawnItem,RespawnTimer,true,0);
}

// Called when the game starts or when spawned
void AItemSpawner::BeginPlay()
{
	Super::BeginPlay();
	
	GetWorldTimerManager().SetTimer(SpawnTimer,this,&ThisClass::SpawnItem,RespawnTimer,true,0);
}

void AItemSpawner::SpawnItem()
{
	if(!ItemClass)
	{
		return;
	}

	if(SpawnedActor)
	{
		return;
	}

	const FVector Location = GetActorLocation() + FVector(0,0,50.f);
	FActorSpawnParameters SpawnParameters = FActorSpawnParameters();
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnedActor = GetWorld()->SpawnActor<ADefaultItem>(ItemClass,Location,FRotator::ZeroRotator,SpawnParameters);
}

