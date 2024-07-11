// Copyright Epic Games, Inc. All Rights Reserved.

#include "SafetyCharacter.h"
#include "SafetyProjectile.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Engine/LocalPlayer.h"
#include "InteractableComponent.h"
#include "SafetyPlayerController.h"
#include "Blueprint/WidgetLayoutLibrary.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// ASafetyCharacter

ASafetyCharacter::ASafetyCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);
		
	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-10.f, 0.f, 60.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	//Mesh1P->SetRelativeRotation(FRotator(0.9f, -19.19f, 5.2f));
	Mesh1P->SetRelativeLocation(FVector(-30.f, 0.f, -150.f));

}

void ASafetyCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();
}

//////////////////////////////////////////////////////////////////////////// Input

void ASafetyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{	
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ASafetyCharacter::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ASafetyCharacter::Look);

		// Interacting
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Triggered, this, &ASafetyCharacter::Interact);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}


void ASafetyCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add movement 
		//AddMovementInput(GetActorForwardVector(), MovementVector.Y);
		AddMovementInput(GetFirstPersonCameraComponent()->GetForwardVector(), MovementVector.Y);
		AddMovementInput(GetActorRightVector(), MovementVector.X);
	}
}

void ASafetyCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void ASafetyCharacter::Interact(const FInputActionValue& Value)
{
	UE_LOG(LogTemp, Display, TEXT("Player interacted"));

	FHitResult HitResult;
	FVector TraceStart = FirstPersonCameraComponent->GetComponentLocation();
	FVector TraceEnd = TraceStart + FirstPersonCameraComponent->GetForwardVector() * 500;
	GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECC_Visibility);


	if (HitResult.GetActor() && HitResult.GetActor()->FindComponentByClass<UInteractableComponent>())
	{
		UInteractableComponent* Interactable = HitResult.GetActor()->FindComponentByClass<UInteractableComponent>();
		Interactable->OnInteract.Broadcast();
	}
	
	if (ActorHasTag(TEXT("SAFE")))
	{
		UWorld* World = GetWorld();
		int32 PPCount = GetWorld()->PostProcessVolumes.Num();
		for (int i = 0; i < PPCount; i++)
		{
			FPostProcessVolumeProperties Volume = World->PostProcessVolumes[i]->GetProperties();
			if (Volume.bIsUnbound)
			{
				FPostProcessSettings* Settings = (FPostProcessSettings*)Volume.Settings;
				Settings->ColorSaturation = FVector4(1.f, 1.f, 1.f, 1.f);
				Settings->ColorContrast = FVector4(1.f, 1.f, 1.f, 1.f);
			}
		}
		
		UWidgetLayoutLibrary::RemoveAllWidgets(GetWorld());
	}

}
