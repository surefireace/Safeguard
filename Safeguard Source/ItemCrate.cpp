// By Donovan Colen
// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemCrate.h"
#include "Engine/World.h"

// Sets default values
AItemCrate::AItemCrate()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	CrateMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CrateMesh"));

	// Set as root component
	//RootComponent = CrateMesh;

	if (ItemInCrate == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("NO Items in crate")); // cant have no items in the crate
	}

}

// spawns the item the crate holds
AItem* AItemCrate::SpawnItem()
{
	// If we have set something to spawn:
	if (ItemInCrate != NULL)
	{
		// Check for a valid World: 
		UWorld* const World = GetWorld();
		if (World)
		{
			// Set the spawn parameters
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			SpawnParams.Instigator = NULL;

			// calculated spawn location
			FVector SpawnLocation = this->GetTransform().GetLocation();

			// Get the rotation for the item
			FRotator SpawnRotation = this->GetTransform().GetRotation().Rotator();

			// spawn the pickup
			AItem* SpawnedItem = World->SpawnActor<AItem>(ItemInCrate, SpawnLocation, SpawnRotation, SpawnParams);
			
			return SpawnedItem;
		}
		else
		{
			UE_LOG(LogTemp, Fatal, TEXT("NO world to spawn items into")); // really bad if tring to spawn items into non existing world
			return nullptr;
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("NO Items in crate to spawn")); // cant have no items in the crate
		return nullptr;
		
	}
}

// Called when the game starts or when spawned
void AItemCrate::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AItemCrate::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

