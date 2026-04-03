// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PlayerHUD.h"

#include "GameObjects/Characters/PlayerCharacter.h"
#include "Inventory/Inventory.h"

bool UPlayerHUD::ToggleInventory()
{
	if (!Inventory->IsVisible())
	{
		Help->SetVisibility(ESlateVisibility::Hidden);
		
		Inventory->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		Inventory->GetOwningPlayer()->SetShowMouseCursor(true);
						
		return true;
	}

	GetOwningPlayer()->SetShowMouseCursor(false);
	Inventory->SetVisibility(ESlateVisibility::Hidden);
	return false;
	
}

bool UPlayerHUD::ToggleHelp()
{
	if (!Help->IsVisible() && !Inventory->IsVisible())
	{
		Help->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		return true;
	}
	
	Help->SetVisibility(ESlateVisibility::Hidden);
	return false;
}

void UPlayerHUD::NativeConstruct()
{
	Super::NativeConstruct();

	if(!GetOwningPlayerPawn<APlayerCharacter>()->OnPlayerTookDamage.IsBoundToObject(this))
	{
		GetOwningPlayerPawn<APlayerCharacter>()->OnPlayerTookDamage.BindUObject(this, &UPlayerHUD::PlayGetHitAnim);
	}
}
