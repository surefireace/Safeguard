// By Donovan Colen
// Fill out your copyright notice in the Description page of Project Settings.// Fill out your copyright notice in the Description page of Project Settings.

#include "CraftingStation.h"
#include "Engine/World.h"

// Sets default values
ACraftingStation::ACraftingStation()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CraftingStationMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CraftingStationMesh"));

	// Set as root component
	RootComponent = CraftingStationMesh;

	bIsCraftingStarted = false;
	bIsRecipeSelected = false;
}

// returns true when a item is successfully added
bool ACraftingStation::AddItem(AItem* item)
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
				item->SetItemDisabled(true);

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

// handles the crafting 
void ACraftingStation::Craft()
{
	// if crafting hasnt started check to see if it can start
	if (!bIsCraftingStarted)
	{
		for (auto i : RecipeList)
		{
			// if the current recipe isnt the current selection and if it can be crafted
			if (!(RecipeBeingCrafted == i) && CanCraft(i))
			{
				FString debugStr = i.Output.Item.GetDefaultObject()->GetName();
				UE_LOG(LogClass, Log, TEXT("crafting started for item %s"), *debugStr);
				RecipeBeingCrafted = i;
				bIsRecipeSelected = true;
				bIsIterationRecipe = true;
				RemainingInteractions = RecipeBeingCrafted.NumberOfInteractions;

				// check if the recipe needs interactions
				if (RecipeBeingCrafted.NumberOfInteractions == 0)
				{
					bIsCraftingStarted = true;
					bIsIterationRecipe = false;
					GetWorldTimerManager().SetTimer(CraftTimer, this, &ACraftingStation::Craft, RecipeBeingCrafted.CraftTime, false);
				}
			}

		}
	}
	else
	{
		// check if the recipe needs interactions
		if (RecipeBeingCrafted.NumberOfInteractions == 0)
		{
			float debugTime = GetWorldTimerManager().GetTimerRemaining(CraftTimer);
			UE_LOG(LogClass, Log, TEXT("time remaining on craft: %f"), debugTime);
		}

		// check if the recipe needs interactions and the timer has hit zero
		if (RecipeBeingCrafted.NumberOfInteractions == 0 && GetWorldTimerManager().GetTimerRemaining(CraftTimer) == 0)
		{
			RemoveRecipeItems();
			bIsRecipeSelected = false;
			bIsCraftingStarted = false;

			// clear the timer
			GetWorldTimerManager().ClearTimer(CraftTimer);

			CreateOutputItem();

		}
		else if(RemainingInteractions <= 0 && bIsIterationRecipe)	// check if the recipe has been interacted with enough
		{
			RemoveRecipeItems();
			bIsRecipeSelected = false;
			bIsCraftingStarted = false;

			CreateOutputItem();
		}
	}

}

// updates RemainingInteractions for when a recipe is actively being crafted by the player
void ACraftingStation::InteractCrafting()
{
	if (bIsRecipeSelected)
	{
		--RemainingInteractions;
		UE_LOG(LogClass, Log, TEXT("hammered item %i hits remaining"), RemainingInteractions);
		bIsCraftingStarted = true;
	}
}

/* returns the crafted item or the last item in inventory.
*  returns nullptr if inventory is empty or crafting is in progress */
AItem* ACraftingStation::GrabItem()
{
	// check to see if not actively crafting and has inventory
	if (!bIsCraftingStarted && Inventory.Num() >= 1)
	{
		AItem* lastItem = Inventory.Pop();

		lastItem->SetItemDisabled(false);

		return lastItem;
	}
	
	UE_LOG(LogClass, Log, TEXT("cant grab a item inventory empty or crafting has started"));
	return nullptr;
}

// returns true if the recipe given is able to be crafted with items in iventory
bool ACraftingStation::CanCraft(FRecipe recipe)
{
	// for each input subitem in recipe 
	for (auto subItem : recipe.Inputs)
	{
		int itemCount = 0;

		// go through inventory to count if the required amount is met 
		for (auto i : Inventory)
		{
			if (i->GetId() == subItem.Item.GetDefaultObject()->GetId())
			{
				++itemCount;
			}
		}
		
		if (itemCount < subItem.Ammount)
		{
			// cant craft recipe if a sub item amount requirement isnt met
			return false;
		}
	}

	// if it reaches here all subItems had their amount requirements met or exceded
	return true;
}

// returns true if a item is a part any recipes in the list
bool ACraftingStation::IsUsedInRecipe(AItem* item)
{
	for (auto i : RecipeList)
	{
		for (auto subItem : i.Inputs)
		{
			if (subItem.Item != NULL)
			{
				if (subItem.Item.GetDefaultObject()->GetId() == item->GetId())
				{
					UE_LOG(LogClass, Log, TEXT("Item found in Recipe"));
					return true;
				}
				else
				{
					auto debugitem = subItem.Item.GetDefaultObject();
					UE_LOG(LogClass, Log, TEXT("Item: %s id: %s not equals item: %s id: %s"),
						*debugitem->GetName(), *debugitem->GetId(), *item->GetName(), *item->GetId());
				}
			}
			else
			{
				UE_LOG(LogClass, Error, TEXT("Item in Recipe is NULL"));
			}
		}
	}
	UE_LOG(LogClass, Warning, TEXT("Item NOT found in Recipe"));
	return false;
}

// removes the items in the inventory that recipe uses
void ACraftingStation::RemoveRecipeItems()
{
	// for each input subitem in recipe 
	for (auto subItem : RecipeBeingCrafted.Inputs)
	{
		int itemCount = subItem.Ammount;
		TArray<AItem*> itemsRemoved;

		// go through inventory and remove the amount required by the recipe
		for (auto i : Inventory)
		{
			if (i->GetId() == subItem.Item.GetDefaultObject()->GetId())
			{
				if (itemCount > 0)
				{
					--itemCount;
					itemsRemoved.Emplace(i);
				}
			}
		}

		for (auto toRemove : itemsRemoved)
		{
			UE_LOG(LogClass, Warning, TEXT("crafting started for item %s"), *toRemove->GetName());
			Inventory.Remove(toRemove);
			toRemove->Destroy();
		}

		if (itemCount != 0)
		{
			// if for some reason itemCount is not zero there is a error
			UE_LOG(LogClass, Error, TEXT("Crafted recipe without the required amount of items"));
		}
	}
}

// creates the item the recipe makes and spawn it
void ACraftingStation::CreateOutputItem()
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

			SpawnedItem->SetItemDisabled(true);

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

// checks and updates current recipe based on new inventory and returns true if recipe updated
bool ACraftingStation::UpdateActiveRecipe()
{
	// loop through recipe list
	for (auto i : RecipeList)
	{
		// if the current recipe isnt the current selection and if it can be crafted
		if (!(RecipeBeingCrafted == i) && CanCraft(i))
		{
			int currentAmount = 0;
			int newAmount = 0;
			for (auto currentRecipeInputs : RecipeBeingCrafted.Inputs)
			{
				currentAmount += currentRecipeInputs.Ammount;
			}

			for (auto newRecipeInputs : i.Inputs)
			{
				newAmount += newRecipeInputs.Ammount;
			}

			// only change to a recipe that uses more items
			if (newAmount > currentAmount)
			{
				FString debugStr = i.Output.Item.GetDefaultObject()->GetName();
				UE_LOG(LogClass, Log, TEXT("crafting updated for item %s"), *debugStr);
				RecipeBeingCrafted = i;
				return true;
			}
			else
			{
				UE_LOG(LogClass, Log, TEXT("new amount %i !> current amount %i"), newAmount, currentAmount);
			}
		}
	}
	return false;
}

// Called when the game starts or when spawned
void ACraftingStation::BeginPlay()
{
	Super::BeginPlay();
	
}

// returns true when inventory if full
bool ACraftingStation::IsInventoryFull()
{
	return Inventory.Num() >= InventorySize;
}

// Called every frame
void ACraftingStation::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	Craft();
}

