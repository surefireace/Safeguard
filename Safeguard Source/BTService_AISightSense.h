// By Donovan Colen
// Fill out your copyright notice in the Description page of Project Settings.// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_AISightSense.generated.h"

/**
 * 
 */
UCLASS()
class SAFEGUARD_API UBTService_AISightSense : public UBTService
{
	GENERATED_BODY()

		//variables ----------
private:
	/** Cached AIController owner of BehaviorTreeComponent. */
	UPROPERTY(Transient)
		AAIController* AIOwner;

	/** Cached actor owner of BehaviorTreeComponent. */
	UPROPERTY(Transient)
		AActor* ActorOwner;

	//functions ----------
private:
	//sets the owner for the service
	virtual void SetOwner(AActor* ActorOwner) override;

public:

	UBTService_AISightSense(const FObjectInitializer& ObjectInitializer);

	// the logic for checking if what the ai sees is a player or not
	void ReceiveTickAI(AAIController* OwnerController, APawn* ControlledPawn, float DeltaSeconds);

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	//needed or linker error uknown reasons
	virtual void OnGameplayTaskActivated(UGameplayTask& Task) override {} 
	virtual void OnGameplayTaskDeactivated(UGameplayTask& Task) override {}
};
