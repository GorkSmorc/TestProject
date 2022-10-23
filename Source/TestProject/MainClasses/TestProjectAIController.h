// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "TestProjectAIController.generated.h"

/**
 * 
 */
UCLASS()
class TESTPROJECT_API ATestProjectAIController : public AAIController
{
	GENERATED_BODY()

	ATestProjectAIController();

protected:

	virtual ETeamAttitude::Type GetTeamAttitudeTowards(const AActor& Other) const override;
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	UAIPerceptionComponent* AIPerceptionComponent = nullptr;
		
};
