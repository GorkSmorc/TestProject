// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CheatManager.h"
#include "TestProjectCheatManager.generated.h"

/**
 * 
 */
UCLASS()
class TESTPROJECT_API UTestProjectCheatManager : public UCheatManager
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable)
	void EnableCheats();

	UFUNCTION(BlueprintCallable)
	void DisableCheats();

	UPROPERTY(BlueprintReadWrite)
	TSubclassOf<class UCheatWidget> CheatWidgetClass = nullptr;

private:

	UPROPERTY()
	UCheatWidget* CheatWidget = nullptr;

	
	
};
