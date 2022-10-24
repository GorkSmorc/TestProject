// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PlayerHUD.h"

#include "GameObjects/Characters/PlayerCharacter.h"

void UPlayerHUD::NativeConstruct()
{
	Super::NativeConstruct();

	if(!GetOwningPlayerPawn<APlayerCharacter>()->OnPlayerTookDamage.IsBoundToObject(this))
	{
		GetOwningPlayerPawn<APlayerCharacter>()->OnPlayerTookDamage.BindUObject(this, &UPlayerHUD::PlayGetHitAnim);
	}
}
