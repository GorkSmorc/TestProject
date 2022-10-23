// Fill out your copyright notice in the Description page of Project Settings.


#include "GameObjects/Items/DefaultItem.h"

// Sets default values
ADefaultItem::ADefaultItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

// Called when the game starts or when spawned
void ADefaultItem::BeginPlay()
{
	Super::BeginPlay();
	
}


