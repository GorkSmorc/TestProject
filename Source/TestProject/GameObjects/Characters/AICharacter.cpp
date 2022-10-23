// Fill out your copyright notice in the Description page of Project Settings.


#include "GameObjects/Characters/AICharacter.h"

#include "AIController.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameObjects/Items/Weapons/GeneralWeapon.h"
#include "Perception/AISense_Damage.h"

AAICharacter::AAICharacter() : Super()
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
	PhysicalAnimationData.OrientationStrength = 500.f;
	PhysicalAnimationData.AngularVelocityStrength = 100.f;
	PhysicalAnimationData.PositionStrength = 500.f;
	PhysicalAnimationData.VelocityStrength = 100.f;
	
	PhysicalAnimationComponent->ApplyPhysicalAnimationSettingsBelow(TEXT("spine_01"), PhysicalAnimationData, false);
	GetMesh()->SetAllBodiesBelowSimulatePhysics(BoneForPA,true,false);
	GetMesh()->SetAllBodiesBelowPhysicsBlendWeight(BoneForPA, 0.f,false,false);

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
	GetMesh()->SetAllBodiesBelowPhysicsBlendWeight(BoneForPA,1.f,false,false);

	PhysicalAnimationComponent->ApplyPhysicalAnimationSettingsBelow("pelvis", FPhysicalAnimationData());
	GetMesh()->SetSimulatePhysics(true);
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

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

void AAICharacter::Destroyed()
{
	if(GetController())
	{
		GetController()->Destroy();
	}
	
	if(Weapon)
	{
		Weapon->Destroy();
	}
	
	Super::Destroyed();
}
