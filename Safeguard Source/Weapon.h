// By Donovan Colen
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "Components/BoxComponent.h"
#include "SafeguardDefines.h"
#include "Weapon.generated.h"

/**
 * 
 */
UCLASS()
class SAFEGUARD_API AWeapon : public AItem
{
	GENERATED_BODY()

// Variables	----------
protected:
	// the character that owns the weapon
	ACharacter* OwningCharacter;

	// the damage the weapon deals
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats", meta = (AllowPrivateAccess = "true"))
		float Damage;

	// the durability of the weapon
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats", meta = (AllowPrivateAccess = "true"))
		float Durability;

	// the name of the bone socket the weapon attaches to
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats", meta = (AllowPrivateAccess = "true"))
		FName SocketName;

	// the damage type of the weapon ie slashing, blunt
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stats", meta = (AllowPrivateAccess = "true"))
		EDamType DamageType;

	// Collision Box
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Collision", meta = (AllowPrivateAccess = "true"))
		class UBoxComponent* CollisionBox;

	
// Functions	----------
public:
	// Sets default values for this actor's properties
	AWeapon();

	// checks to see if weapon hit something other than owner and does damage calculations
	void NotifyHit(class UPrimitiveComponent* MyComp, AActor* Other, class UPrimitiveComponent* OtherComp,
		bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit) override;

	// sets the weapons owner
	void SetWeaponOwner(ACharacter* owner);

	// returns the weapons owner
	ACharacter* GetWeaponOwner() const;

	// returns the socket name for the weapon
	FORCEINLINE FName GetSocketName() const { return SocketName; }

	// Returns Collision Box subobject
	FORCEINLINE class UBoxComponent* GetCollisionBox() const { return CollisionBox; }
};
