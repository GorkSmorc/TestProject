// Fill out your copyright notice in the Description page of Project Settings.


#include "MainClasses/TestProjectAIController.h"

#include "Perception/AIPerceptionComponent.h"

ATestProjectAIController::ATestProjectAIController() : Super()
{
	AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>("AIPerceptionComponent");
}

ETeamAttitude::Type ATestProjectAIController::GetTeamAttitudeTowards(const AActor& Other) const
{
	const APawn* OtherPawn = Cast<APawn>(&Other);
	if(!OtherPawn)
	{
		return  ETeamAttitude::Neutral;
	}

	if(OtherPawn->IsBotControlled())
	{
		return ETeamAttitude::Hostile;
	}
	else if(OtherPawn->IsPlayerControlled())
	{
		return ETeamAttitude::Friendly;
	}

	
	return Super::GetTeamAttitudeTowards(Other);
}
