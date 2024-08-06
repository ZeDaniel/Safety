// Fill out your copyright notice in the Description page of Project Settings.


#include "HeldItemDestructionField.h"
#include "SafetyCharacter.h"
#include "Components/BoxComponent.h"
#include "ItemComponent.h"

AHeldItemDestructionField::AHeldItemDestructionField()
{
	DefaultMaterial = CreateDefaultSubobject<UMaterialInstance>(TEXT("Default Material"));
	RejectMaterial = CreateDefaultSubobject<UMaterialInstance>(TEXT("Reject Material"));
	AcceptMaterial = CreateDefaultSubobject<UMaterialInstance>(TEXT("Accept Material"));
}

void AHeldItemDestructionField::IncreaseDestroyedItems()
{
	NumDestroyedItems++;
	CheckActionThreshold();
}

void AHeldItemDestructionField::BeginPlay()
{
	Super::BeginPlay();

}

void AHeldItemDestructionField::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnComponentBeginOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

	//Only do something if gate is active
	if (bIsActive)
	{
		//Check for player character and if an item is already held
		if (ASafetyCharacter* Character = Cast<ASafetyCharacter>(OtherActor))
		{
			UE_LOG(LogTemp, Display, TEXT("Valid Character walked into HELD ITEM volume"));
			if (Character->IsHoldingItem())
			{
				UE_LOG(LogTemp, Display, TEXT("Held item found"));
				UItemComponent* FoundItem;
				Character->GetInstanceComponents().FindItemByClass<UItemComponent>(&FoundItem);

				//Prevent destruction if item has exempt tag
				if (!FoundItem->ComponentHasTag(ExemptItemTag))
				{
					CheckForPassThroughItem(FoundItem);

					FoundItem->HandleDestruction();

					NumDestroyedItems++;

					CheckActionThreshold();
				}
			}
			else
			{
				UE_LOG(LogTemp, Display, TEXT("No held item, do nothing"));
			}

		}
	}
}

void AHeldItemDestructionField::SetMaterial(UMaterialInstance* NewMaterial)
{
	MeshComp->SetMaterial(0, NewMaterial);

	TArray<USceneComponent*> MeshChildren = MeshComp->GetAttachChildren();

	for (USceneComponent* Child : MeshChildren)
	{
		if (UMeshComponent* ChildMesh = Cast<UMeshComponent>(Child))
		{
			ChildMesh->SetMaterial(0, NewMaterial);
		}
	}
}

void AHeldItemDestructionField::CheckForPassThroughItem(UItemComponent* Item)
{
	if (Item->ComponentHasTag(PassThroughItemTag))
	{
		SetMaterial(AcceptMaterial);

		FTimerHandle AcceptTimer;
		FTimerDelegate AcceptTimerDelegate = FTimerDelegate::CreateUObject(this, &AHeldItemDestructionField::EnablePlayerPassthrough);
		GetWorldTimerManager().SetTimer(AcceptTimer, AcceptTimerDelegate, AcceptanceTimerLength, false);
	}
	else
	{
		SetMaterial(RejectMaterial);

		FTimerHandle RejectTimer;
		FTimerDelegate RejectTimerDelegate = FTimerDelegate::CreateUObject(this, &AHeldItemDestructionField::SetMaterial, DefaultMaterial);
		GetWorldTimerManager().SetTimer(RejectTimer, RejectTimerDelegate, RejectionTimerLength, false);
	}
}

void AHeldItemDestructionField::CheckActionThreshold()
{
	if (NumDestroyedItems >= PerformActionThreshold)
	{
		PerformAction();
		NumDestroyedItems = 0;
	}	
}

void AHeldItemDestructionField::EnablePlayerPassthrough()
{
	MeshComp->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
	
	bIsActive = false;
}
