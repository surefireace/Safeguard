// By Donovan Colen
// Fill out your copyright notice in the Description page of Project Settings.// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "AnimNotifyState_Attack.generated.h"

/**
 * 
 */
UCLASS()
class SAFEGUARD_API UAnimNotifyState_Attack : public UAnimNotifyState
{
	GENERATED_BODY()
	
public:
	// turns on the weapon collision when the animation notify start frame is hit
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration) override;

	// turns off the weapon collision when the animation notify end frame is hit
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
};
