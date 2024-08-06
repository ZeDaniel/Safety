// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ItemDestructionField.generated.h"

UCLASS()
class SAFETY_API AItemDestructionField : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItemDestructionField();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintImplementableEvent)
	void PerformAction();

	UPROPERTY(EditAnywhere, Category="Item")
	int PerformActionThreshold = -1;

	UPROPERTY(EditAnywhere)
	class UBoxComponent* BoxComp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UStaticMeshComponent* MeshComp;

	UPROPERTY(EditAnywhere)
	FName ExemptItemTag;

	UFUNCTION()
	virtual void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	int NumDestroyedItems = 0;

	bool bIsActive = true;


private:

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
