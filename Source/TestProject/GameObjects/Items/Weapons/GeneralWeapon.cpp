// Fill out your copyright notice in the Description page of Project Settings.


#include "GeneralWeapon.h"

#include "Kismet/GameplayStatics.h"

#include "Components/ArrowComponent.h"
#include "MainClasses/TestProjectGameInstance.h"
#include "Particles/ParticleSystemComponent.h"

AGeneralWeapon::AGeneralWeapon()
{
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	
	FP_Gun = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FP_Gun"));
	FP_Gun->bCastDynamicShadow = false;
	FP_Gun->CastShadow = false;
	FP_Gun->SetupAttachment(RootComponent);
	

	Arrow = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow"));
	Arrow->SetupAttachment(FP_Gun);

	Particle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Particle"));
	Particle->SetupAttachment(FP_Gun);
}

void AGeneralWeapon::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if(Dispersion > 0)
	{
		Dispersion -= DispersionReduce;
		if(Dispersion < 0)
		{
			Dispersion = 0.f;
		}
	}
}

void AGeneralWeapon::BeginPlay()
{
	Super::BeginPlay();

	Ammo = WeaponStats.MaxAmmo;
	
}

void AGeneralWeapon::CreateTrace()
{
	if(bReloading)
	{
		return;
	}
	if(!GetAttachParentActor())
	{
		return;
	}
		
	if(Ammo <= 0)
	{
		if(!EmptySoundCooldown)
		{
			EmptySoundCooldown = true;
			UGameplayStatics::PlaySoundAtLocation(this, WeaponStats.EmptySound, GetActorLocation(),GetGameInstance<UTestProjectGameInstance>()->GetSoundVolume());
			FTimerHandle ReloadSoundTimer;
			OnWeaponFired.ExecuteIfBound();
			GetWorldTimerManager().SetTimer(ReloadSoundTimer,[this]()	{ EmptySoundCooldown = false; },0.5f,false); 
		}

		return;
	}	

	UAnimInstance* AnimInstance = FP_Gun->GetAnimInstance();
	if (AnimInstance != nullptr)
	{
		AnimInstance->Montage_Play(WeaponStats.WeaponShootAnim, 1.f);
	}
	
	FVector Start;
	FVector End;
	
	const APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this,0);
	if(PlayerController->GetPawn() == GetAttachParentActor())
	{
		FVector DispersionDirection = FMath::VRand();
		DispersionDirection.Normalize();
		Start =  PlayerController->PlayerCameraManager->GetCameraLocation();
		End = PlayerController->PlayerCameraManager->GetActorForwardVector() * 5000 + Start + Dispersion * DispersionDirection;
	}
	else
	{
		Start = GetAttachParentActor()->GetActorLocation() + FVector(0,0,30.f);
		End = Arrow->GetForwardVector() * 5000.f + Start;
	}
	
	const TArray<AActor*> ActorsToIgnore { GetAttachParentActor() };
	FHitResult OutHit;
	
	if(UKismetSystemLibrary::LineTraceSingle(this,Start,End,TraceTypeQuery3,false,ActorsToIgnore,EDrawDebugTrace::None,OutHit,true))
	{
		if (OutHit.Actor != nullptr) 
		{
			const int32 Damage = WeaponStats.Damage + FMath::RandRange(-5,5);
			UGameplayStatics::ApplyPointDamage(OutHit.Actor.Get(),Damage,OutHit.TraceStart,OutHit,GetAttachParentActor()->GetInstigatorController(),GetAttachParentActor(),UDamageType::StaticClass());
			CreateHitDecal(OutHit);
		}
	}

	if(APawn* CastedPawn = Cast<APawn>(GetAttachParentActor()))
	{
		MakeNoise(1,CastedPawn,GetActorLocation(),0,"Shoot");
	}
	
	Dispersion += WeaponStats.DispersionPerShot;
	if(Dispersion > WeaponStats.MaxDispersion)
	{
		Dispersion = WeaponStats.MaxDispersion;
	}

	if(!bIsUnlimitedAmmoEnabled)
		Ammo--;
	
	Particle->Activate(true);
	OnAmmoUpdated.Broadcast(Ammo);
	OnWeaponFired.ExecuteIfBound();
	bCooldown = true;
	
	if (WeaponStats.FireSound != nullptr)
	{
		UGameplayStatics::PlaySoundAtLocation(this, WeaponStats.FireSound, GetActorLocation(),GetGameInstance<UTestProjectGameInstance>()->GetSoundVolume());
	}
}

void AGeneralWeapon::OnReloadEnded(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload)
{
	if(NotifyName != "Reloaded")
		return;
	
	if(UAnimInstance* AnimInstance = FP_Gun->GetAnimInstance())
	{
		AnimInstance->OnPlayMontageNotifyBegin.RemoveDynamic(this, &ThisClass::OnReloadEnded);
		Ammo += AmmoToLoad;
		OnAmmoUpdated.Broadcast(Ammo);
		bReloading = false;
		OnWeaponReloaded.Broadcast(AmmoToLoad);
		AmmoToLoad = 0;
	}
}

void AGeneralWeapon::Reloading(int32 _AmmoToLoad)
{
	AmmoToLoad = _AmmoToLoad;
	UAnimInstance* AnimInstance = FP_Gun->GetAnimInstance();
	if (AnimInstance == nullptr)
	{
		UE_LOG(LogTemp,Error,TEXT("%s Reloading: AnimInstance == nullptr!"), *GetName())
		return;
	}

	const bool bIsFull = Ammo == 0;
	UAnimMontage* MontageToPlay = bIsFull ? WeaponStats.WeaponReloadAnimFullEmpty : WeaponStats.WeaponReloadAnim;

	AnimInstance->OnPlayMontageNotifyBegin.AddUniqueDynamic(this, &ThisClass::OnReloadEnded);
	AnimInstance->Montage_Play(MontageToPlay, 1.f);
	bReloading = true;
	
	OnWeaponReloading.ExecuteIfBound(bIsFull);
	
}

void AGeneralWeapon::OnFire()
{
	if(bCooldown)
		return;
	
	switch (WeaponType)
	{
	case EWeaponType::Pistol:
		{
			CreateTrace();

			FTimerHandle CooldownTimer;
			FTimerDelegate TimerDelegate;
			TimerDelegate.BindWeakLambda(this, [this](){ bCooldown = false; });
			GetWorldTimerManager().SetTimer(CooldownTimer,TimerDelegate,WeaponStats.FireRate,false);
			break;
		}
	case EWeaponType::AssaultRifle:
		{
			if(!GetWorldTimerManager().IsTimerActive(ShootTimer))
			{
				GetWorldTimerManager().SetTimer(ShootTimer,this,&ThisClass::CreateTrace,WeaponStats.FireRate,true, 0);
			}
			break;
		}
	}

	DispersionReduce = WeaponStats.DispersionPerShot / WeaponStats.DispersionReduceDivisor;
	
}

void AGeneralWeapon::OnStopFire()
{
	FTimerManager& TimerManager = GetWorldTimerManager();
	if(TimerManager.IsTimerActive(ShootTimer))
	{
		FTimerHandle WaitForLastShootTimer;
		FTimerDelegate TimerDelegate;
		TimerDelegate.BindWeakLambda(this, [this]()
		{
			bCooldown = false;
		});
		TimerManager.SetTimer(WaitForLastShootTimer,TimerDelegate,TimerManager.GetTimerRemaining(ShootTimer),false);
		TimerManager.ClearTimer(ShootTimer);
	
	}

	DispersionReduce = WeaponStats.DispersionPerShot / (WeaponStats.DispersionReduceDivisor / 2);
}


