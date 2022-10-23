// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DefaultItem.generated.h"

UCLASS()
class TESTPROJECT_API ADefaultItem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADefaultItem();

	UPROPERTY()
	int32 StackSize = 255;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
};
