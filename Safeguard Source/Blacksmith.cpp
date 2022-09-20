// By Donovan Colen
// Fill out your copyright notice in the Description page of Project Settings.// Fill out your copyright notice in the Description page of Project Settings.

#include "Blacksmith.h"
#include "ItemCrate.h"
#include "CraftingStation.h"
#include "ScrapBin.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"

// Sets default values
ABlacksmith::ABlacksmith()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Create the collection box
	CollectionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollectionBox"));
	CollectionBox->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)

}

// Called to bind functionality to input
void ABlacksmith::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAction("Grab/Drop", IE_Pressed, this, &ABlacksmith::PickUpDrop);
	PlayerInputComponent->BindAction("Interact/Attack", IE_Pressed, this, &ABlacksmith::Interact);

	PlayerInputComponent->BindAxis("MoveForward", this, &ABlacksmith::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ABlacksmith::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &ABlacksmith::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &ABlacksmith::LookUpAtRate);
}


// Called when the game starts or when spawned
void ABlacksmith::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABlacksmith::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// to pick up or drop items when key is pressed
void ABlacksmith::PickUpDrop()
{
	UE_LOG(LogTemp, Warning, TEXT("pickup/drop")); // for testing
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Dropped item"));

	// if not holding item try to pick one up else drop currently held item
	if (HeldItem == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("tried to pick up a item")); // for testing

		// Get all overlapping Actors and store them in an array
		TArray<AActor*> CollectedActors;
		CollectionBox->GetOverlappingActors(CollectedActors);

		// For each Actor we collected
		for (int count = 0; count < CollectedActors.Num(); ++count)
		{
			// Cast the actor to AItem
			AItem* const TestPickup = Cast<AItem>(CollectedActors[count]);

			// If the cast is successful and the pickup is valid and active 
			if (TestPickup)
			{
				UE_LOG(LogTemp, Warning, TEXT("picked up a item")); // for testing

				HeldItem = TestPickup;

				// turn off physics
				HeldItem->GetMesh()->SetSimulatePhysics(false);
				HeldItem->SetActorEnableCollision(false);

				// do calculation for object location
				FVector grabLocation = this->GetTransform().GetLocation();
				grabLocation += this->GetActorForwardVector() * GrabbedObjectDistance;
				HeldItem->SetActorLocationAndRotation(grabLocation, this->GetActorQuat());

				// attach the object
				HeldItem->GetMesh()->AttachToComponent(CollectionBox, FAttachmentTransformRules::KeepWorldTransform);

				// Call the Items's Collect function
				HeldItem->Collect();
				
				// no need to continue already holding item
				break;
			}

			// Cast the actor to AItemCrate
			AItemCrate* const TestSpawn = Cast<AItemCrate>(CollectedActors[count]);

			// if the item is valid
			if (TestSpawn)
			{
				UE_LOG(LogTemp, Warning, TEXT("Used item Crate")); // for testing

				HeldItem = TestSpawn->SpawnItem();

				// turn off physics
				HeldItem->GetMesh()->SetSimulatePhysics(false);
				HeldItem->SetActorEnableCollision(false);

				// do calculation for object location
				FVector grabLocation = this->GetTransform().GetLocation();
				grabLocation += this->GetActorForwardVector() * GrabbedObjectDistance;
				HeldItem->SetActorLocationAndRotation(grabLocation, this->GetActorQuat());

				// attach the object
				HeldItem->GetMesh()->AttachToComponent(CollectionBox, FAttachmentTransformRules::KeepWorldTransform);

				// Call the Items's Collect function
				HeldItem->Collect();

				// no need to continue already holding item
				break;
			}

			// Cast the actor to AItemCrate
			ACraftingStation* const TestGrab = Cast<ACraftingStation>(CollectedActors[count]);

			// if the item is valid
			if (TestGrab)
			{
				UE_LOG(LogTemp, Warning, TEXT("Used grab item crafting station")); // for testing

				HeldItem = TestGrab->GrabItem();

				if (HeldItem == nullptr)
				{
					UE_LOG(LogTemp, Warning, TEXT("Failed to grab item crafting station")); // for testing
					break;
				}

				// turn off physics
				HeldItem->GetMesh()->SetSimulatePhysics(false);
				HeldItem->SetActorEnableCollision(false);

				// do calculation for object location
				FVector grabLocation = this->GetTransform().GetLocation();
				grabLocation += this->GetActorForwardVector() * GrabbedObjectDistance;
				HeldItem->SetActorLocationAndRotation(grabLocation, this->GetActorQuat());

				// attach the object
				HeldItem->GetMesh()->AttachToComponent(CollectionBox, FAttachmentTransformRules::KeepWorldTransform);

				// Call the Items's Collect function
				HeldItem->Collect();

				// no need to continue already holding item
				break;
			}


		}

	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("droped the item")); // for testing

		// Get all overlapping Actors and store them in an array
		TArray<AActor*> CollectedActors;
		CollectionBox->GetOverlappingActors(CollectedActors);

		// For each Actor we collected
		for (int count = 0; count < CollectedActors.Num(); ++count)
		{
			// Cast the actor to AScrapBin
			AScrapBin* const TestDrop = Cast<AScrapBin>(CollectedActors[count]);

			// If the cast is successful trash the item
			if (TestDrop)
			{
				UE_LOG(LogTemp, Warning, TEXT("Dropped item in Scrap Bin")); // for testing

				// unattach the object
				HeldItem->GetMesh()->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);

				// turn on physics
				HeldItem->GetMesh()->SetSimulatePhysics(true);
				HeldItem->SetActorEnableCollision(true);

				// trash the item
				TestDrop->DestroyItem(HeldItem);

				//clear variable
				HeldItem = nullptr;

				// no need to continue already dropped item
				break;
			}

			// Cast the actor to ACraftingStation
			ACraftingStation* const TestCraft = Cast<ACraftingStation>(CollectedActors[count]);

			// If the cast is successful try to add the item to crafting station
			if (TestCraft)
			{
				UE_LOG(LogTemp, Warning, TEXT("Dropped item in Crafting Station")); // for testing

				// test to see if the item was added
				if (TestCraft->AddItem(HeldItem))
				{
					// unattach the object
					HeldItem->GetMesh()->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);

					//clear variable
					HeldItem = nullptr;

					// no need to continue already dropped item
					break;

				}
			}
		}

		// if the item wasnt trown into a scrap bin drop it
		if (HeldItem != nullptr)
		{
			// unattach the object
			HeldItem->GetMesh()->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);

			// turn on physics
			HeldItem->GetMesh()->SetSimulatePhysics(true);
			HeldItem->SetActorEnableCollision(true);

			//clear variable
			HeldItem = nullptr;
		}
	}

}

// to interact when key is pressed
void ABlacksmith::Interact()
{
	UE_LOG(LogTemp, Warning, TEXT("try Interact")); // for testing

	// Get all overlapping Actors and store them in an array
	TArray<AActor*> CollectedActors;
	CollectionBox->GetOverlappingActors(CollectedActors);

	// For each Actor we collected
	for (int count = 0; count < CollectedActors.Num(); ++count)
	{
		// Cast the actor to ACraftingStation
		ACraftingStation* const TestCraft = Cast<ACraftingStation>(CollectedActors[count]);

		// If the cast is successful try to interact with crafting station
		if (TestCraft)
		{
			UE_LOG(LogTemp, Warning, TEXT("Interacted with Crafting Station")); // for testing

			TestCraft->InteractCrafting();

			// found crafting station no need to continue looking
			break;
		}
	}

}

/**
 * Called via input to turn at a given rate.
 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
 */
void ABlacksmith::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

/**
 * Called via input to turn look up/down at a given rate.
 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
 */
void ABlacksmith::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

/** Called for forwards/backward input */
void ABlacksmith::MoveForward(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

/** Called for side to side input */
void ABlacksmith::MoveRight(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}
