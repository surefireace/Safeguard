// By Donovan Colen
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "GameFramework/Actor.h"
#include "ItemCrate.generated.h"

UCLASS()
class SAFEGUARD_API AItemCrate : public AActor
{
	GENERATED_BODY()
// variables	----------
private:
	// mesh component for the item
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item", meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* CrateMesh;

	// the item that the crate contains
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class AItem> ItemInCrate;


// functions	----------
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Sets default values for this actor's properties
	AItemCrate();

	// spawns the item the crate holds
	AItem* SpawnItem();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Returns mesh for the item
	FORCEINLINE class UStaticMeshComponent* GetMesh() const { return CrateMesh; }

};
