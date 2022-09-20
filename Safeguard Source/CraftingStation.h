// By Donovan Colen
// Fill out your copyright notice in the Description page of Project Settings.// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "SafeguardDefines.h"
#include "Containers/Array.h"
#include "GameFramework/Actor.h"
#include "CraftingStation.generated.h"

// struct for a item and it's amount
USTRUCT(BlueprintType)
struct FItemAmounts
{
	GENERATED_BODY()

	// the item needed for recipe
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemAmounts")
	TSubclassOf<AItem> Item;

	// should never be zero
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemAmounts")
	int32 Ammount;

};

// struct for a recipes used for crafting 
USTRUCT(BlueprintType)
struct FRecipe
{
	GENERATED_BODY()

	// the items and their amounts for the recipe
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recipe")
	TArray<FItemAmounts> Inputs;

	// used for the crafting timmer. I.E. the furnace or water tub
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recipe")
	float CraftTime;

	/* used for the interaction stations. I.E. the workbench or anvil
	*  if number is zero the crafting time will be used instead */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recipe")
	int NumberOfInteractions;

	// the items that are the result of the crafting
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recipe")
	FItemAmounts Output;

	// clears all the recipes
	void Clear()
	{
		CraftTime = 0;
		NumberOfInteractions = 0;
		for (auto i : Inputs)
		{
			i.Ammount = 0;
			i.Item = nullptr;
		}
		Output.Ammount = 0;
		Output.Item = nullptr;
	}
};

// overloaded == comparison for FItemAmounts
FORCEINLINE bool operator== (const FItemAmounts& i1, const FItemAmounts& i2)
{
	return (i1.Item == i2.Item && i1.Ammount == i2.Ammount);
}

// overloaded == comparison for FRecipe
FORCEINLINE bool operator== (const FRecipe& r1, const FRecipe& r2)
{
	return (r1.Inputs == r2.Inputs && r1.Output == r2.Output);
}

UCLASS()
class SAFEGUARD_API ACraftingStation : public AActor
{
	GENERATED_BODY()

// variables	----------
private:
	// mesh component for the item
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CraftingStation", meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* CraftingStationMesh;

protected:
	// the size of the crafting stations inventory 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory", meta = (AllowPrivateAccess = "true"))
	int InventorySize;

	// the array to store the items in the inventory 
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory", meta = (AllowPrivateAccess = "false"))
	TArray<AItem*> Inventory;

	// the array to store all the valid recipes
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recipes", meta = (AllowPrivateAccess = "true"))
	TArray<FRecipe> RecipeList;

	// stores the recipe that is currently being made
	FRecipe RecipeBeingCrafted;

	// too keep track of how many inputs have already been done by the player
	int RemainingInteractions;

	// to tell when the crafting process has started
	bool bIsCraftingStarted;

	// to tell if a recipe is selected for crafting
	bool bIsRecipeSelected;

	// to check if the recipe needs interactions
	bool bIsIterationRecipe;

	/* allows the adding of additional objects during time based crafting.
	*  NOT to be used with interation based crafting*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recipes", meta = (AllowPrivateAccess = "true"))
	bool bFurnaceMode;

	// the timer for crafting
	FTimerHandle CraftTimer;

// functions	----------
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// returns true if inventory is full
	bool IsInventoryFull();

	// returns true if the recipe given is able to be crafted with items in iventory
	bool CanCraft(FRecipe recipe);

	// returns true if a item is a part any recipes in the list
	bool IsUsedInRecipe(AItem* item);

	// removes the items in the inventory that recipe uses
	void RemoveRecipeItems();

	// creates the item the recipe makes and spawn it
	virtual void CreateOutputItem();

	// checks and updates current recipe based on new inventory and returns true if recipe updated
	bool UpdateActiveRecipe();

public:
	// Sets default values for this actor's properties
	ACraftingStation();

	// returns true when a item is successfully added
	virtual bool AddItem(AItem* item);

	// handles the crafting 
	virtual void Craft();

	// updates RemainingInteractions for when a recipe is actively being crafted by the player
	void InteractCrafting();

	/* returns the crafted item or the last item in inventory.
	*  returns nullptr if inventory is empty or crafting is in progress */
	AItem* GrabItem();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Returns mesh for the item
	FORCEINLINE class UStaticMeshComponent* GetMesh() const { return CraftingStationMesh; }

};
