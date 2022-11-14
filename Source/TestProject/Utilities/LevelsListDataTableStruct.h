// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "LevelsListDataTableStruct.generated.h"

/**
 * 
 */

USTRUCT(BlueprintType)
struct FLevelsListDataTableStruct : public FTableRowBase
{
	GENERATED_BODY()
	
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	TSoftObjectPtr<UWorld> Level;
		
	FLevelsListDataTableStruct() :
		Level(nullptr)
	{}
		
};

