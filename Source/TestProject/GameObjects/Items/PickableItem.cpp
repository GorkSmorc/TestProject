// Fill out your copyright notice in the Description page of Project Settings.


#include "GameObjects/Items/PickableItem.h"

#include "Components/InventoryManager.h"
#include "GameObjects/Characters/TestProjectCharacter.h"
#include "MainClasses/TestProjectPlayerState.h"

APickableItem::APickableItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMeshComponent->SetupAttachment(RootComponent);

	StaticMeshComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	StaticMeshComponent->SetCollisionResponseToChannel(ECC_Pawn,ECR_Overlap);
}

// Called when the game starts or when spawned
void APickableItem::BeginPlay()
{
	Super::BeginPlay();

	if(StaticMeshComponent->GetStaticMesh())
	{
		StaticMeshComponent->OnComponentBeginOverlap.AddUniqueDynamic(this, &ThisClass::OnMeshOverlapped);
	}
}

void APickableItem::OnPickedUp(ATestProjectCharacter* OverlappedCharacter)
{
	if(OverlappedCharacter->IsPlayerControlled())
	{
		if(OverlappedCharacter->GetPlayerState<ATestProjectPlayerState>()->GetInventoryManager()->AddItem(GetClass(),Count))
		{
			Destroy();
		}
	}
}

void APickableItem::OnMeshOverlapped(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                     UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(ATestProjectCharacter* CastedChar = Cast<ATestProjectCharacter>(OtherActor))
	{
		OnPickedUp(CastedChar);
	}
}

// Called every frame
void APickableItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(StaticMeshComponent)
		StaticMeshComponent->AddWorldRotation(FRotator(0,RotateSpeed,0));
}

