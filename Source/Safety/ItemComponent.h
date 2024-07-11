// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "ItemComponent.generated.h"

/**
 * 
 */

UCLASS(Blueprintable, BlueprintType, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SAFETY_API UItemComponent : public UStaticMeshComponent
{
	GENERATED_BODY()

public:
	/** Attaches the actor to a FirstPersonCharacter */
	UFUNCTION(BlueprintCallable, Category = "Item")
	bool AttachItem(class ASafetyCharacter* TargetCharacter);

	/** Destroys the item.  */
	UFUNCTION(BlueprintCallable, Category = "Item")
	void HandleDestruction();

protected:
	UPROPERTY(EditAnywhere, Category = "Happiness")
	FVector4 SaturationPickupVector;

	UPROPERTY(EditAnywhere, Category = "Happiness")
	FVector4 SaturationPutdownVector;

	UPROPERTY(EditAnywhere, Category = "Happiness")
	FVector4 ContrastPickupVector;

	UPROPERTY(EditAnywhere, Category = "Happiness")
	FVector4 ContrastPutdownVector;

	UPROPERTY(EditAnywhere, Category = "Happiness")
	bool ResetOnPickup = false;

	UPROPERTY(EditAnywhere, Category = "Happiness")
	bool ResetOnPutdown = false;

private:
	/** The Character holding this weapon*/
	ASafetyCharacter* Character;

	void UpdateSaturationPickup();

	void UpdateSaturationPutdown();
	
};
