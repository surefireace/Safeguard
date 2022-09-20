// By Donovan Colen
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Misc/Guid.h"
#include "GameFramework/Actor.h"
#include "Item.generated.h"

UCLASS()
class SAFEGUARD_API AItem : public AActor
{
	GENERATED_BODY()

	// Variables	----------
private:
	// mesh component for the item
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item", meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* ItemMesh;

protected:
	// A unique string id specific to the type of item.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ID", meta = (AllowPrivateAccess = "false"))
	FString Id;
	
	// Functions	----------
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Sets default values for this actor's properties
	AItem();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// returns the item id
	FString GetId();

	// turns off rendering and collision of item if passed true
	void SetItemDisabled(bool disable);

	// Function to call when the pickup is collected
	UFUNCTION(BlueprintNativeEvent)
	void Collect();		// for use in blueprints
	virtual void Collect_Implementation();

	// Returns mesh for the item
	FORCEINLINE class UStaticMeshComponent* GetMesh() const { return ItemMesh; }

};
