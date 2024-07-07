// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemComponent.h"
#include "SafetyCharacter.h"

bool UItemComponent::AttachItem(ASafetyCharacter* TargetCharacter)
{
	Character = TargetCharacter;

	// Check that the character is valid, and has no item component yet
	// TODO: Support for multiple items
	if (Character == nullptr || Character->GetInstanceComponents().FindItemByClass<UItemComponent>())
	{
		return false;
	}

	// Attach the item to the First Person Character
	FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
	AttachToComponent(Character->GetMesh1P(), AttachmentRules, FName(TEXT("GripPoint")));

	// add the item as an instance component to the character
	Character->AddInstanceComponent(this);


	return true;
}
