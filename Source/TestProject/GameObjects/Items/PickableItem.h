// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DefaultItem.h"
#include "PickableItem.generated.h"

UCLASS()
class TESTPROJECT_API APickableItem : public ADefaultItem
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APickableItem();
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	int32 GetCount() const { return Count; }

	UFUNCTION(BlueprintCallable)
	UTexture2D* GetImage() const { return Image; }
		
	UPROPERTY(EditAnywhere)
	float RotateSpeed = 1.f;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void OnPickedUp(class ATestProjectCharacter* OverlappedCharacter);

	UPROPERTY(BlueprintReadOnly,EditAnywhere)
	int32 Count = 1;

	UPROPERTY(BlueprintReadOnly,EditAnywhere)
	UTexture2D* Image = nullptr;
	
	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* StaticMeshComponent = nullptr;

private:

	UFUNCTION()
	void OnMeshOverlapped(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
												int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	
};
