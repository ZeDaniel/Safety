// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemDestructionField.h"
#include "HeldItemDestructionField.generated.h"

/**
 * 
 */
UCLASS()
class SAFETY_API AHeldItemDestructionField : public AItemDestructionField
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AHeldItemDestructionField();

	UFUNCTION(BlueprintCallable)
	void IncreaseDestroyedItems();

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMaterialInstance* DefaultMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMaterialInstance* RejectMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMaterialInstance* AcceptMaterial;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "Item")
	FName PassThroughItemTag;

	UPROPERTY(EditAnywhere, Category = "Item")
	float RejectionTimerLength = 1.0f;

	UPROPERTY(EditAnywhere, Category = "Item")
	float AcceptanceTimerLength = 1.0f;

	void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
	
private:

	void SetMaterial(UMaterialInstance* NewMaterial);

	void CheckForPassThroughItem(class UItemComponent* Item);

	void CheckActionThreshold();
};
