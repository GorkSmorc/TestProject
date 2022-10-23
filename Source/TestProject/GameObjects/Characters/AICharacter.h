// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameObjects/Characters/TestProjectCharacter.h"
#include "AICharacter.generated.h"

/**
 * 
 */
DECLARE_DELEGATE_OneParam(FOnBotDead, AActor* Bot);

UCLASS()
class TESTPROJECT_API AAICharacter : public ATestProjectCharacter
{
	GENERATED_BODY()

	AAICharacter();

public:
	
	FOnBotDead OnBotDead;

protected:

	virtual void BeginPlay() override;		
	virtual float InternalTakePointDamage(float Damage, struct FPointDamageEvent const& PointDamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	virtual void Reloading() override;
	virtual void Destroyed() override;

	UFUNCTION(BlueprintImplementableEvent)
	void MeshHit();
	
	UFUNCTION(BlueprintImplementableEvent)
	void SpawnRandomWeapon();

	UFUNCTION(BlueprintImplementableEvent)
	void StopAllPATimelines();
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UPhysicalAnimationComponent* PhysicalAnimationComponent = nullptr;
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	FName BoneForPA = "spine_01";
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	int32 MaxHealth = 100;

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite)
	int32 Health = 100;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	bool bIsDead = false;
	
	UFUNCTION(BlueprintCallable)
	void Death();
	
};

