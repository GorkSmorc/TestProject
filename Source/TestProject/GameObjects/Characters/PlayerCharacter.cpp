// Fill out your copyright notice in the Description page of Project Settings.


#include "GameObjects/Characters/PlayerCharacter.h"

#include "AICharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InventoryManager.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpectatorPawn.h"
#include "GameObjects/Items/Weapons/GeneralWeapon.h"
#include "Kismet/KismetSystemLibrary.h"
#include "MainClasses/TestProjectGameInstance.h"
#include "MainClasses/TestProjectGameMode.h"
#include "MainClasses/TestProjectHUD.h"
#include "MainClasses/TestProjectPlayerState.h"
#include "UI/Inventory/Inventory.h"
#include "Utilites/InventoryStruct.h"
#include "Utilites/UseInterface.h"

APlayerCharacter::APlayerCharacter() : Super()
{
	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;

	SkeletalMesh = Mesh1P;
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	if(Weapon == nullptr)
	{
		Mesh1P->SetHiddenInGame(true);
	}

	GetWorld()->GetAuthGameMode<ATestProjectGameMode>()->OnPlayerSpawned(this);
}

void APlayerCharacter::PossessedBy(AController* NewController) //Equip weapon from loaded inventory
{
	Super::PossessedBy(NewController);
    
	if(const ATestProjectPlayerState* CastedPlayerState = NewController->GetPlayerState<ATestProjectPlayerState>())
	{
		auto Lambda = [this,CastedPlayerState]()
		{
			if(CastedPlayerState->GetInventoryManager()->GetWeaponInventory().Num()>0)
			{
				const FWeaponInventoryStruct FirstWeapon = CastedPlayerState->GetInventoryManager()->GetWeaponInventory()[0];
				EquipWeapon(FirstWeapon.Weapon.LoadSynchronous(), FirstWeapon.Ammo);
			}
		};

		if(CastedPlayerState->GetInventoryManager()->bIsInventoryLoaded)
		{
			Lambda();
		}
		else
		{
			CastedPlayerState->GetInventoryManager()->OnInventoryLoaded.AddWeakLambda(this, Lambda);
		}
	}
}

void APlayerCharacter::Destroyed()
{
	Super::Destroyed();

	if(Weapon)
	{
		Weapon->Destroy();
	}
}

void APlayerCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// set up gameplay key bindings
	check(PlayerInputComponent);

	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &APlayerCharacter::Run);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &APlayerCharacter::StopRunning);
	
	// Bind jump events
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	// Bind fire event
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &APlayerCharacter::OnFire);
	PlayerInputComponent->BindAction("Fire", IE_Released, this, &APlayerCharacter::OnStopFire);

	//Bind reload event
	PlayerInputComponent->BindAction("Reload", IE_Pressed, this, &APlayerCharacter::Reloading);

	//Bind switch weapon event
	PlayerInputComponent->BindAction("NextWeapon", IE_Pressed, this, &APlayerCharacter::OnNextWeapon);
	PlayerInputComponent->BindAction("PreviousWeapon", IE_Pressed, this, &APlayerCharacter::OnPreviousWeapon);

	//Bind crouch event
	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &APlayerCharacter::OnCrouchStarted);
	PlayerInputComponent->BindAction("Crouch", IE_Released, this, &APlayerCharacter::OnCrouchEnded);

	//Bind open inventory event
	PlayerInputComponent->BindAction("Inventory", IE_Pressed, this, &APlayerCharacter::OnInventoryClicked);
	//Bind Interact event
	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &APlayerCharacter::OnInteractClicked);
	//Bind help button
	PlayerInputComponent->BindAction("Help", IE_Pressed, this, &APlayerCharacter::OnHelpClicked);
	
	// Bind movement events
	PlayerInputComponent->BindAxis("MoveForward", this, &APlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &APlayerCharacter::MoveRight);
	
	PlayerInputComponent->BindAxis("Turn", this, &APlayerCharacter::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &APlayerCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APlayerCharacter::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &APlayerCharacter::LookUpAtRate);
}

void APlayerCharacter::AddControllerYawInput(float Val)
{
	if(bInWidget)
	{
		return;
	}
	
	Super::AddControllerYawInput(Val * GetGameInstance<UTestProjectGameInstance>()->GetMouseSensitivity());
}

void APlayerCharacter::AddControllerPitchInput(float Val)
{
	if(bInWidget)
	{
		return;
	}
	
	Super::AddControllerPitchInput(Val * GetGameInstance<UTestProjectGameInstance>()->GetMouseSensitivity());
}

void APlayerCharacter::OnFire()
{
	if(bInWidget)
	{
		return;
	}
	
	Super::OnFire();
}

void APlayerCharacter::Reloading()
{
	if(bInWidget)
	{
		return;
	}
	
	if(Weapon == nullptr)
	{
		return;
	}

	if(Weapon->GetWeaponStats().MaxAmmo == Weapon->GetAmmo() || Weapon->bReloading)
	{
		return;
	}

	FInventoryItem FoundedSlot;
	UInventoryManager* InventoryManager = GetPlayerState<ATestProjectPlayerState>()->GetInventoryManager();
	if(InventoryManager->FindItem(Weapon->GetWeaponStats().AmmoClass, FoundedSlot))
	{
		int32 AmmoToLoad = Weapon->GetWeaponStats().MaxAmmo - Weapon->GetAmmo();
		if(AmmoToLoad > FoundedSlot.Count)
		{
			AmmoToLoad = FoundedSlot.Count;
		}
		
		Weapon->OnWeaponReloaded.AddUniqueDynamic(this, &ThisClass::RemoveAmmoFromInventory);
		Weapon->OnWeaponReloading.BindWeakLambda(this,[this](const bool bIsFull)
		{
			if(UAnimInstance* AnimInstance = SkeletalMesh->GetAnimInstance())
			{
				UAnimMontage* MontageToPlay = bIsFull ? Weapon->GetWeaponStats().CharacterReloadAnimFullEmpty : Weapon->GetWeaponStats().CharacterReloadAnim;
				AnimInstance->Montage_Play(MontageToPlay, 1.f);
			}
		});

		Weapon->Reloading(AmmoToLoad);
	}
}

float APlayerCharacter::InternalTakePointDamage(float Damage, FPointDamageEvent const& PointDamageEvent,
                                                AController* EventInstigator, AActor* DamageCauser)
{	
	if(ATestProjectPlayerState* CastedPlayerState = Cast<ATestProjectPlayerState>(GetPlayerState()))
	{
		CastedPlayerState->SetHealth(CastedPlayerState->GetHealth() - Damage);
		if(CastedPlayerState->GetHealth() <= 0)
		{
			Death(DamageCauser);
			UE_LOG(LogTemp,Warning,TEXT("~~~DEAD"))
			OnPlayerDead.ExecuteIfBound();
		}
	}
	
	return Damage;
}

void APlayerCharacter::OnInventoryClicked()
{
	if(ATestProjectHUD* HUD = GetController<APlayerController>()->GetHUD<ATestProjectHUD>())
	{
		HUD->ShowHideInventory();
		GetCharacterMovement()->SetMovementMode(HUD->GetInventory()->IsInViewport() ? MOVE_None : MOVE_Walking);
		bInWidget = HUD->GetInventory()->IsInViewport();
		if(bInWidget)
		{
			OnStopFire();
			OnCrouchEnded();
		}
		else
		{
			GetController<APlayerController>()->SetInputMode(FInputModeGameOnly());
		}
		
	}
}

void APlayerCharacter::OnHelpClicked()
{
	if(ATestProjectHUD* HUD = GetController<APlayerController>()->GetHUD<ATestProjectHUD>())
	{
		HUD->ShowHideHelp();		
	}
}

void APlayerCharacter::OnCrouchStarted()
{
	if(bInWidget)
	{
		return;
	}
	
	Crouch();
	if(Weapon)
	{
		Weapon->SetMaxDispersion(Weapon->GetWeaponStats().MaxDispersion / 2);
	}
}

void APlayerCharacter::OnCrouchEnded()
{
	UnCrouch();
	if(Weapon)
	{
		Weapon->SetMaxDispersion(Weapon->GetWeaponStats().MaxDispersion * 2);
	}
}

void APlayerCharacter::Death(AActor* Killer)
{
	if(APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if(AAICharacter* AICharacter = Cast<AAICharacter>(Killer))
		{
			APawn* Spectator = GetWorld()->SpawnActor<APawn>(ASpectatorPawn::StaticClass(),GetActorTransform(),FActorSpawnParameters());
			PlayerController->Possess(Spectator);
			PlayerController->SetViewTargetWithBlend(AICharacter,0.2f);
		}
	}
		
	Destroy();
}

void APlayerCharacter::OnNextWeapon()
{
	if(bSwitchingWeapon || bInWidget)
	{
		return;
	}
		
	NewWeapon = GetPlayerState<ATestProjectPlayerState>()->GetInventoryManager()->GetNextWeapons(Weapon);
	if(NewWeapon.Weapon == nullptr)
	{
		return;
	}

	bSwitchingWeapon = true;
	Mesh1P->GetAnimInstance()->OnPlayMontageNotifyBegin.AddUniqueDynamic(this,&ThisClass::SwitchWeapon);
	Mesh1P->GetAnimInstance()->Montage_Play(Weapon->GetWeaponStats().HolsterAnim);
}

void APlayerCharacter::OnPreviousWeapon()
{
	if(bSwitchingWeapon || bInWidget)
	{
		return;
	}
		
	NewWeapon = GetPlayerState<ATestProjectPlayerState>()->GetInventoryManager()->GetPreviousWeapons(Weapon);
	if(NewWeapon.Weapon == nullptr)
	{
		return;
	}

	bSwitchingWeapon = true;
	Mesh1P->GetAnimInstance()->OnPlayMontageNotifyBegin.AddUniqueDynamic(this,&ThisClass::SwitchWeapon);
	Mesh1P->GetAnimInstance()->Montage_Play(Weapon->GetWeaponStats().HolsterAnim);
}

void APlayerCharacter::SwitchWeapon(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload)
{
	if(NotifyName == "Hidden")
	{
		Mesh1P->GetAnimInstance()->OnPlayMontageNotifyBegin.RemoveDynamic(this,&ThisClass::SwitchWeapon);
		Weapon->Destroy();
		
		EquipWeapon(NewWeapon.Weapon.LoadSynchronous(),NewWeapon.Ammo);
		bSwitchingWeapon = false;
	}
	
}

void APlayerCharacter::RemoveAmmoFromInventory(int32 AmmoLoaded)
{
	Weapon->OnWeaponReloaded.RemoveDynamic(this, &ThisClass::RemoveAmmoFromInventory);
	UInventoryManager* InventoryManager = GetPlayerState<ATestProjectPlayerState>()->GetInventoryManager();
	InventoryManager->RemoveItem(Weapon->GetWeaponStats().AmmoClass, AmmoLoaded);
}

void APlayerCharacter::OnInteractClicked()
{
	const FVector Start = FirstPersonCameraComponent->GetComponentLocation();
	const FVector End = GetActorForwardVector() * 300 + Start;
	const TArray<AActor*> ActorsToIgnore { Weapon };

	FHitResult OutHit;
	if(UKismetSystemLibrary::LineTraceSingle(this,Start,End,TraceTypeQuery1,false,ActorsToIgnore,EDrawDebugTrace::None, OutHit,true))
	{
		if(OutHit.Actor->GetClass()->ImplementsInterface(UUseInterface::StaticClass()))
		{
			IUseInterface::Execute_Use(OutHit.Actor.Get());
		}
	}
}

void APlayerCharacter::TurnAtRate(float Rate)
{
	if(!bInWidget)
	{
		// calculate delta for this frame from the rate information
		AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
	}
}

void APlayerCharacter::LookUpAtRate(float Rate)
{
	if(!bInWidget)
	{
		// calculate delta for this frame from the rate information
		AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
	}
}