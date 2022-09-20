// By Donovan Colen
// Fill out your copyright notice in the Description page of Project Settings.// Fill out your copyright notice in the Description page of Project Settings.

#include "Item.h"

// Sets default values
AItem::AItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	// create the mesh
	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMesh"));

	// Set as root component
	RootComponent = ItemMesh;

	// enable physics for the item
	ItemMesh->SetSimulatePhysics(true);
	this->SetActorEnableCollision(true);
}

// returns the item id
FString AItem::GetId()
{
	return Id;
}

// turns off rendering and collision of item if passed true
void AItem::SetItemDisabled(bool disable)
{
	// disable physics for the item
	ItemMesh->SetSimulatePhysics(!disable);
	this->SetActorEnableCollision(!disable);
	this->SetActorHiddenInGame(disable);

}

// Called when the game starts or when spawned
void AItem::BeginPlay()
{
	Super::BeginPlay();
}

// Function to call when the pickup is collected
void AItem::Collect_Implementation()
{
	// Log a debug message
	FString ItemDebugString = GetName();
	UE_LOG(LogClass, Log, TEXT("You have Picked up %s"), *ItemDebugString);

	UE_LOG(LogClass, Log, TEXT("with a ID of: %s"), *Id);

}

// Called every frame
void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

