// By Donovan Colen
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CraftingStation.h"
#include "Components/BoxComponent.h"
#include "VisualCraftingStation.generated.h"

// this class is for crafting stations where the object being cafted is visible by the player ie hammering hot ingots
UCLASS()
class SAFEGUARD_API AVisualCraftingStation : public ACraftingStation
{
	GENERATED_BODY()

// variables	----------
private:
	// Collection Box
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Collecting", meta = (AllowPrivateAccess = "true"))
		class UBoxComponent* PlacementBox;

protected:
	// to change the distance of the placed object
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item", meta = (AllowPrivateAccess = "true"))
		float PlacedObjectDistance;

	// to change the pitch of the placed object
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item", meta = (AllowPrivateAccess = "true"))
		float PlacedObjectPitch;

	// to change the yaw of the placed object
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item", meta = (AllowPrivateAccess = "true"))
		float PlacedObjectYaw;

	// to change the roll of the placed object
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item", meta = (AllowPrivateAccess = "true"))
		float PlacedObjectRoll;

// functions	----------
protected:

	// creates the item the recipe makes
	virtual void CreateOutputItem() override;

public:
	// Sets default values for this actor's properties
	AVisualCraftingStation();

	// returns true when a item is successfully added
	virtual bool AddItem(AItem* item) override; 

	
};
