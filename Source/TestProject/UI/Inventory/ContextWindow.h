// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ContextWindow.generated.h"

/**
 * 
 */
UCLASS()
class TESTPROJECT_API UContextWindow : public UUserWidget
{
	GENERATED_BODY()

protected:
		
	virtual void NativeConstruct() override;

public:

	UFUNCTION(BlueprintCallable)
	void Init(class UInventorySlot* InventorySlot);

	UFUNCTION(BlueprintCallable)
	UInventorySlot* GetClickedSlot() const { return ClickedSlot; }

private:
	
	UFUNCTION()
	void OnUseItem();
	
	UFUNCTION()
	void OnDeleteItem();

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget, AllowPrivateAccess))
	class UButton* UseButton = nullptr;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget, AllowPrivateAccess))
	UButton* DeleteButton = nullptr;
		
	UPROPERTY()
	UInventorySlot* ClickedSlot = nullptr;
	
};
