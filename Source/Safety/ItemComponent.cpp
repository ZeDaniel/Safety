// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemComponent.h"
#include "SafetyCharacter.h"
#include "TP_PickUpComponent.h"
#include "Kismet/GameplayStatics.h"

bool UItemComponent::AttachItem(ASafetyCharacter* TargetCharacter)
{
	Character = TargetCharacter;

	// Check that the character is valid, and has no item component yet
	// TODO: Support for multiple items
	if (Character == nullptr || Character->IsHoldingItem())
	{
		return false;
	}

	// Attach the item to the First Person Character
	FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
	AttachToComponent(Character->GetMesh1P(), AttachmentRules, FName(TEXT("GripPoint")));

	// add the item as an instance component to the character
	Character->AddInstanceComponent(this);

	Character->SetIsHoldingItem(true);

	UpdateSaturationPickup();


	return true;
}

void UItemComponent::HandleDestruction()
{
	if (Character)
	{
		UpdateSaturationPutdown();
		if (UTP_PickUpComponent* PickUpComp = Cast<UTP_PickUpComponent>(GetChildComponent(0)))
		{
			PickUpComp->OnPutDown.Broadcast(Character);
			Character->SetIsHoldingItem(false);
			UE_LOG(LogTemp, Display, TEXT("IsHoldingItem: %d"), Character->IsHoldingItem());

			Character->RemoveInstanceComponent(this);
		}

	}
	else
	{
		UpdateSaturationPickup();
	}

	//TODO: Fizzle anim
	GetOwner()->Destroy();

}

void UItemComponent::UpdateSaturationPickup()
{
	UWorld* World = GetWorld();
	int32 PPCount = GetWorld()->PostProcessVolumes.Num();
	for (int i = 0; i < PPCount; i++)
	{
		FPostProcessVolumeProperties Volume = World->PostProcessVolumes[i]->GetProperties();
		if (Volume.bIsUnbound)
		{
			if (ResetOnPickup)
			{
				FPostProcessSettings* Settings = (FPostProcessSettings*)Volume.Settings;
				Settings->ColorSaturation = FVector4(1.f, 1.f, 1.f, 1.f);
				Settings->ColorContrast = FVector4(1.f, 1.f, 1.f, 1.f);
			}
			else
			{
				FPostProcessSettings* Settings = (FPostProcessSettings*)Volume.Settings;
				Settings->ColorSaturation += SaturationPickupVector;
				Settings->ColorContrast += ContrastPickupVector;
			}
		}
	}
}

void UItemComponent::UpdateSaturationPutdown()
{
	UWorld* World = GetWorld();
	int32 PPCount = GetWorld()->PostProcessVolumes.Num();
	for (int i = 0; i < PPCount; i++)
	{
		FPostProcessVolumeProperties Volume = World->PostProcessVolumes[i]->GetProperties();
		if (Volume.bIsUnbound)
		{
			if (ResetOnPutdown)
			{
				FPostProcessSettings* Settings = (FPostProcessSettings*)Volume.Settings;
				Settings->ColorSaturation = FVector4(1.f, 1.f, 1.f, 1.f);
				Settings->ColorContrast = FVector4(1.f, 1.f, 1.f, 1.f);
			}
			else
			{
				FPostProcessSettings* Settings = (FPostProcessSettings*)Volume.Settings;
				Settings->ColorSaturation += SaturationPutdownVector;
				Settings->ColorContrast += SaturationPutdownVector;
			}
		}
	}
}
