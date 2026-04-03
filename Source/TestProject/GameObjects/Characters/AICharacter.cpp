// Fill out your copyright notice in the Description page of Project Settings.


#include "GameObjects/Characters/AICharacter.h"

#include <Engine/DamageEvents.h>
#include <GameFramework/CharacterMovementComponent.h>
#include <PhysicsEngine/PhysicalAnimationComponent.h>

#include "Components/CapsuleComponent.h"
#include "GameObjects/Items/Weapons/GeneralWeapon.h"
#include "Perception/AISense_Damage.h"

AAICharacter::AAICharacter()
{
	PhysicalAnimationComponent = CreateDefaultSubobject<UPhysicalAnimationComponent>(TEXT("Physical Animation Component"));

	SkeletalMesh = GetMesh();
}

float AAICharacter::InternalTakePointDamage(float Damage, FPointDamageEvent const& PointDamageEvent,
	AController* EventInstigator, AActor* DamageCauser)
{
	if(bIsDead || Cast<AAICharacter>(DamageCauser))
	{
		return Super::InternalTakePointDamage(Damage, PointDamageEvent, EventInstigator, DamageCauser);
	}

	UAISense_Damage::ReportDamageEvent(this,this,DamageCauser,Damage,DamageCauser->GetActorLocation(),PointDamageEvent.HitInfo.Location);
	MeshHit();
	
	if(PointDamageEvent.HitInfo.BoneName.ToString().Contains("head"))
	{
		Death();
	}
	else
	{			
		Health -= Damage;
		if(Health <= 0)
		{
			Death();
		}
	}
	
	return Damage;
}


void AAICharacter::BeginPlay()
{
	Super::BeginPlay();

	PhysicalAnimationComponent->SetSkeletalMeshComponent(GetMesh());

	FPhysicalAnimationData PhysicalAnimationData;
	PhysicalAnimationData.bIsLocalSimulation = false;
	PhysicalAnimationData.OrientationStrength = 700.f;
	PhysicalAnimationData.AngularVelocityStrength = 300.f;
	PhysicalAnimationData.PositionStrength = 500.f;
	PhysicalAnimationData.VelocityStrength = 200.f;
	
	PhysicalAnimationComponent->ApplyPhysicalAnimationSettingsBelow(BoneForPA, PhysicalAnimationData, true);

	GetMesh()->SetAllBodiesBelowSimulatePhysics(BoneForPA,true,true);
	GetMesh()->SetAllBodiesBelowPhysicsBlendWeight(BoneForPA, 0.f,false,true);

	SpawnRandomWeapon();
}

void AAICharacter::Death()
{	
	bIsDead = true;
	GetController()->Destroy();	

	if(Weapon)
	{
		Weapon->Destroy();
	}
	
	StopAllPATimelines();
	PhysicalAnimationComponent->ApplyPhysicalAnimationSettingsBelow(BoneForPA, FPhysicalAnimationData(), false);
	GetMesh()->SetAllBodiesBelowSimulatePhysics(BoneForPA,false,true);
	
	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetCharacterMovement()->SetMovementMode(MOVE_None);

	OnBotDead.ExecuteIfBound(this);
}


void AAICharacter::Reloading()
{
	if(Weapon == nullptr)
	{
		return;
	}
	
	Weapon->OnWeaponReloading.BindWeakLambda(this,[this](const bool bIsFull)
	{
		if(UAnimInstance* AnimInstance = SkeletalMesh->GetAnimInstance())
		{
			UAnimMontage* MontageToPlay = bIsFull ? Weapon->GetWeaponStats().CharacterReloadAnimFullEmpty : Weapon->GetWeaponStats().CharacterReloadAnim;
			AnimInstance->Montage_Play(MontageToPlay, 1.f);
		}
	});

	Weapon->Reloading(Weapon->GetWeaponStats().MaxAmmo);
}

void AAICharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	
	OnBotDead.Unbind();
	if(GetController())
	{
		GetController()->Destroy();
	}
	
	if(Weapon)
	{
		Weapon->Destroy();
	}
}