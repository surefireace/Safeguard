// By Donovan Colen
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Character.h"
#include "Blacksmith.generated.h"

UCLASS()
class SAFEGUARD_API ABlacksmith : public ACharacter
{
	GENERATED_BODY()

// Variables	----------
private:
	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

	// Collection Box
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Collecting", meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* CollectionBox;

protected:
	// the item the blacksmith is holding
	AItem* HeldItem;

	// the distance for how far a held object should be in front of the blacksmith
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item", meta = (AllowPrivateAccess = "true"))
	float GrabbedObjectDistance;

public:
	
	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	float BaseLookUpRate;

// Functions	----------
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	/**
	 * Called via input to turn at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface


public:	
	// Sets default values for this character's properties
	ABlacksmith();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// to pick up or drop items when key is pressed
	UFUNCTION(BlueprintCallable, Category = "item")
	void PickUpDrop();

	// to interact when key is pressed
	UFUNCTION(BlueprintCallable, Category = "item")
	void Interact();

	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
	// Returns CollectionBox subobject
	FORCEINLINE class UBoxComponent* GetCollectionBox() const { return CollectionBox; }

};
