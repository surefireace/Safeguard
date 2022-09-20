// By Donovan Colen
// Fill out your copyright notice in the Description page of Project Settings.// Fill out your copyright notice in the Description page of Project Settings.

#include "GameFramework/Actor.h"
#include "Guard.h"
#include "Weapon.h"
#include "AnimNotifyState_Attack.h"

// turns on the weapon collision when the animation notify start frame is hit
void UAnimNotifyState_Attack::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
    UE_LOG(LogTemp, Warning, TEXT("Collide ON")); // for testing

    if (!MeshComp)
    {
        return;
    }

    AActor* owner = MeshComp->GetOwner();
    AGuard* guard = Cast<AGuard>(owner);
    
    //check guard is valid
    if (guard)
    {
        AWeapon* weapon = guard->GetHeldWeapon();

        //check weapon is valid
        if (!weapon)
        {
            return;
        }
        
        weapon->SetActorEnableCollision(true);
        weapon->GetCollisionBox()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    }
}

// turns off the weapon collision when the animation notify end frame is hit
void UAnimNotifyState_Attack::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
    UE_LOG(LogTemp, Warning, TEXT("Collide OFF")); // for testing

    if (!MeshComp)
    {
        return;
    }

    AActor* owner = MeshComp->GetOwner();
    AGuard* guard = Cast<AGuard>(owner);

    //check guard is valid
    if (guard)
    {
        AWeapon* weapon = guard->GetHeldWeapon();

        //check weapon is valid
        if (!weapon)
        {
            return;
        }

        weapon->GetCollisionBox()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
        weapon->SetActorEnableCollision(false);
    }

}
