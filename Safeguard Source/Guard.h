// By Donovan Colen
// Fill out your copyright notice in the Description page of Project Settings.// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon.h"
#include "Components/ChildActorComponent.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Character.h"
#include "Guard.generated.h"

UCLASS()
class SAFEGUARD_API AGuard : public ACharacter
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

	// perception source so ai can see player
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AISenseStimuli", meta = (AllowPrivateAccess = "true"))
		class UAIPerceptionStimuliSourceComponent* AISightStimuli;

protected:
	// the weapon the player is holding
	AWeapon* HeldWeapon;

	// how much health the guard has to start with
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats", meta = (AllowPrivateAccess = "true"))
		float DefaultHealth;

	// the current health of the guard
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stats", meta = (AllowPrivateAccess = "true"))
		float CurrentHealth;

	// WIP does nothing right now
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stats", meta = (AllowPrivateAccess = "true"))
		float AttackSpeed;

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

	// called when the player attacks
	void Attack();

	// called when player is hit by a enemy and does damage calculations
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
		class AController* EventInstigator, class AActor* DamageCauser) override;

public:	
	// Sets default values for this character's properties
	AGuard();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// to pick up or drop Weapons when key is pressed
	UFUNCTION(BlueprintCallable, Category = "weapon")
		void PickUpDrop();

	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
	// Returns CollectionBox subobject
	FORCEINLINE class UBoxComponent* GetCollectionBox() const { return CollectionBox; }
	// Returns the AI sense stimuli component
	FORCEINLINE class UAIPerceptionStimuliSourceComponent* GetPerceptionComponent() const { return AISightStimuli; }
	/** Returns CameraBoom subobject **/
	FORCEINLINE class AWeapon* GetHeldWeapon() const { return HeldWeapon; }

};
