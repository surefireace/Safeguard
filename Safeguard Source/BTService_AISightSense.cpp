// By Donovan Colen
// Fill out your copyright notice in the Description page of Project Settings.// Fill out your copyright notice in the Description page of Project Settings.

#include "BTService_AISightSense.h"
#include "BehaviorTree/BTFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "BlueprintNodeHelpers.h"
#include "AIController.h"
#include "MonsterAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Monster.h"
#include "Guard.h"



UBTService_AISightSense::UBTService_AISightSense(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
    // all blueprint based nodes must create instances
    bCreateNodeInstance = true;
}


void UBTService_AISightSense::SetOwner(AActor* InActorOwner)
{
	ActorOwner = InActorOwner;
	AIOwner = Cast<AAIController>(InActorOwner);
}

// the logic for checking if what the ai sees is a player or not
void UBTService_AISightSense::ReceiveTickAI(AAIController* OwnerController, APawn* ControlledPawn, float DeltaSeconds)
{
	AMonster* monster = Cast<AMonster>(ControlledPawn);
	if (monster)
	{
		TSubclassOf<UAISense_Sight> senseClass;
		TArray<AActor*> sensedActors;
		monster->GetPerceptionComponent()->GetPerceivedActors(senseClass, sensedActors);

		// loop through all sensed actors
		for (auto actor : sensedActors)
		{				
			AMonsterAIController* monsterController = Cast<AMonsterAIController>(OwnerController);
			// if a monster controller if found among them
			if (monsterController)
			{	
				AActor* testActor = Cast<AGuard>(actor);
				// check if actor is a guard
				if(testActor)
				{
					UE_LOG(LogTemp, Log, TEXT("FOUND YOU")); // for testing

					//can see the guard so set the value
					monsterController->GetBlackboardComponent()->SetValueAsObject(monsterController->GetBlackboardKey(), testActor);
					break;
				}
				else
				{
					// reset the value because lost sight of the guard
					monsterController->GetBlackboardComponent()->SetValueAsObject(monsterController->GetBlackboardKey(), nullptr);

				}
			}
		}
	}
}

void UBTService_AISightSense::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	if (AIOwner != nullptr && FBTNodeBPImplementationHelper::AISpecific)
	{
		ReceiveTickAI(AIOwner, AIOwner->GetPawn(), DeltaSeconds);
	}
}


