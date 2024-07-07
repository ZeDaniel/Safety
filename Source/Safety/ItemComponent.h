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

private:
	/** The Character holding this weapon*/
	ASafetyCharacter* Character;
	
};
