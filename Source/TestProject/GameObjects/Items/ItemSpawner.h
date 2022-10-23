// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ItemSpawner.generated.h"

UCLASS()
class TESTPROJECT_API AItemSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItemSpawner();

	UFUNCTION(BlueprintCallable)
	void ResetSpawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	float RespawnTimer = 30.f;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class ADefaultItem> ItemClass = nullptr;
	
private:
	
	UFUNCTION()
	void SpawnItem();

	UPROPERTY()
	AActor* SpawnedActor = nullptr;

	FTimerHandle SpawnTimer;
};
