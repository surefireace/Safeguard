// By Donovan Colen
// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

AWeapon::AWeapon()
    : Damage(0),
    DamageType(EDamType::k_None),
    Durability(0)
{
    CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollectionBox"));
    CollisionBox->SetupAttachment(GetMesh());

    CollisionBox->OnComponentHit.AddDynamic(this, &AWeapon::OnHit);
}

// checks to see if weapon hit something other than owner and does damage calculations
void AWeapon::NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp,
    bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
{
    ACharacter* otherCharacter = Cast<ACharacter>(Other);
    
    //check if the character is valid
    if (otherCharacter)
    {
        UE_LOG(LogTemp, Warning, TEXT("Weapon Hit character %s"), *otherCharacter->GetFName().ToString()); // for testing

        TSubclassOf<UDamageType> type; //will change in future
        UGameplayStatics::ApplyDamage(otherCharacter, Damage, nullptr, this, type);
    }
    

}

// sets the weapons owner
void AWeapon::SetWeaponOwner(ACharacter* owner)
{
    OwningCharacter = owner;
}

// returns the weapons owner
ACharacter* AWeapon::GetWeaponOwner() const
{
    return OwningCharacter;
}
