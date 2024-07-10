// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemDestructionField.h"
#include "Components/BoxComponent.h"
#include "SafetyCharacter.h"

// Sets default values
AItemDestructionField::AItemDestructionField()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Component"));
	SetRootComponent(BoxComp);

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh Component"));
	MeshComp->SetupAttachment(BoxComp);

}

// Called when the game starts or when spawned
void AItemDestructionField::BeginPlay()
{
	Super::BeginPlay();
	
	BoxComp->OnComponentBeginOverlap.AddDynamic(this, &AItemDestructionField::OnComponentBeginOverlap);
}

void AItemDestructionField::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	UE_LOG(LogTemp, Display, TEXT("Character walked into volume"));
}

// Called every frame
void AItemDestructionField::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

