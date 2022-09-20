// By Donovan Colen
// Fill out your copyright notice in the Description page of Project Settings.


#include "VisualCraftingStation.h"
#include "Engine/World.h"

AVisualCraftingStation::AVisualCraftingStation()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create the placement box
	PlacementBox = CreateDefaultSubobject<UBoxComponent>(TEXT("PlacementBox"));
	PlacementBox->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	bIsCraftingStarted = false;
	bIsRecipeSelected = false;

}

// returns true when a item is successfully added
bool AVisualCraftingStation::AddItem(AItem * item)
{
	// if inventory is already full return false because cant add item
	if (IsInventoryFull())
	{
		return false;
	}

	// check if no crafting has started or if crafting station is in furnace mode
	if (!bIsCraftingStarted || bFurnaceMode)
	{
		// check if the item already exists in the inventory
		if (Inventory.Contains(item))
		{
			UE_LOG(LogClass, Log, TEXT("Item already in Inventory"));

			return false;
		}
		else
		{
			// check if the item is used in a recipe for the crafting station
			if (IsUsedInRecipe(item))
			{
				UE_LOG(LogClass, Log, TEXT("adding new item to Inventory"));
				Inventory.Emplace(item);

				// turn off physics
				item->GetMesh()->SetSimulatePhysics(false);
				item->SetActorEnableCollision(false);

				// do calculation for object location
				FVector grabLocation = this->GetTransform().GetLocation();
				grabLocation += this->GetActorUpVector() * PlacedObjectDistance;
				FRotator actorRot = this->GetActorRotation();
				FQuat newQuat = FQuat(FRotator(actorRot.Pitch + PlacedObjectPitch, actorRot.Yaw + PlacedObjectYaw, actorRot.Roll + PlacedObjectRoll));
				item->SetActorLocationAndRotation(grabLocation, newQuat);

				// attach the object
				item->GetMesh()->AttachToComponent(PlacementBox, FAttachmentTransformRules::KeepWorldTransform);

				// check to see if crafting station should act as a furnace
				if (bFurnaceMode == true)
				{
					// check the timer is active
					if (GetWorldTimerManager().IsTimerActive(CraftTimer))
					{
						float timeRemaining = 0;
						timeRemaining = GetWorldTimerManager().GetTimerRemaining(CraftTimer);

						// check if the recipe changed
						if (UpdateActiveRecipe())
						{
							timeRemaining += (RecipeBeingCrafted.CraftTime / 2);
							GetWorldTimerManager().SetTimer(CraftTimer, this, &ACraftingStation::Craft, timeRemaining, false);
						}
						else
						{
							UE_LOG(LogClass, Log, TEXT("Recipe not updated"));
						}
					}
					else
					{
						UE_LOG(LogClass, Log, TEXT("timmer not active"));
					}
				}

				return true;
			}
			else
			{
				UE_LOG(LogClass, Log, TEXT("cant add items that are not part of a recipe"));
				return false;
			}
		}
	}
	UE_LOG(LogClass, Log, TEXT("cant add items inventory is Full or crafting has already started"));
	return false;
}

// creates the item the recipe makes
void AVisualCraftingStation::CreateOutputItem()
{
	// If we have set something to spawn:
	if (RecipeBeingCrafted.Output.Item != NULL)
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
			AItem* SpawnedItem = World->SpawnActor<AItem>(RecipeBeingCrafted.Output.Item, SpawnLocation, SpawnRotation, SpawnParams);

			// turn off physics
			SpawnedItem->GetMesh()->SetSimulatePhysics(false);
			SpawnedItem->SetActorEnableCollision(false);

			// do calculation for object location
			FVector grabLocation = this->GetTransform().GetLocation();
			grabLocation += this->GetActorUpVector() * PlacedObjectDistance;
			FRotator actorRot = this->GetActorRotation();
			FQuat newQuat = FQuat(FRotator(actorRot.Pitch + PlacedObjectPitch, actorRot.Yaw + PlacedObjectYaw, actorRot.Roll + PlacedObjectRoll));
			SpawnedItem->SetActorLocationAndRotation(grabLocation, newQuat);

			// attach the object
			SpawnedItem->GetMesh()->AttachToComponent(PlacementBox, FAttachmentTransformRules::KeepWorldTransform);

			Inventory.Emplace(SpawnedItem);

			//clear recipe being crafted
			RecipeBeingCrafted.Clear();
		}
	}
	else
	{
		// cant have null item for output
		UE_LOG(LogClass, Error, TEXT("Recipe output is Null"));
	}
}
