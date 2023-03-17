// Fill out your copyright notice in the Description page of Project Settings.


#include "BasicZombie.h"

#include <Components/ArrowComponent.h>
#include <Components/CapsuleComponent.h>
#include <Engine/DamageEvents.h>
#include <GameFramework/CharacterMovementComponent.h>

void ABasicZombie::BeginPlay()
{
    Super::BeginPlay();

    FScriptDelegate Delegate;
    Delegate.BindUFunction(this, "OnCapsuleBeginOverlap");
    GetCapsuleComponent()->OnComponentBeginOverlap.Add(Delegate);
    
    FScriptDelegate TakePointDamageDelegate;
    TakePointDamageDelegate.BindUFunction(this, FName("TakePointDamage"));
    OnTakePointDamage.Add(TakePointDamageDelegate);
}

void ABasicZombie::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (bEnableMovement)
        AddMovementInput(GetArrowComponent()->GetForwardVector(), 1.0f);

    if (GetCharacterMovement()->IsMovingOnGround())
    {
        if (Health <= 0)
        {
            // TODO: Make an OnDeath BlueprintNativeEvent
            Destroy();
        }
    }
}

float ABasicZombie::TakeDamage(float Damage, const FDamageEvent& DamageEvent, AController* EventInstigator, AActor* Causer)
{
    float DamageTaken = APaperCharacter::TakeDamage(Damage, DamageEvent, EventInstigator, Causer);
    DamageTaken = FMath::Min(Health, Damage);
    Health -= DamageTaken;
    return DamageTaken;
}

void ABasicZombie::OnCapsuleBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (OtherActor->CanBeDamaged())
    {
        FPointDamageEvent DamageEvent;
        DamageEvent.ShotDirection = (OtherActor->GetActorLocation() - GetActorLocation()).GetSafeNormal();
        DamageEvent.Damage = DamageInflicted;
        DamageEvent.HitInfo = SweepResult;

        OtherActor->TakeDamage(DamageInflicted, DamageEvent, GetController(), this);
        UE_LOG(LogTemp, Log, TEXT("Damaged Actor"));
    }
}

void ABasicZombie::TakePointDamage(AActor* DamagedActor, float Damage, AController* InstigatedBy, FVector HitLocation, UPrimitiveComponent* FHitComponent, FName BoneName, FVector ShotFromDirection, const UDamageType* DamageType, AActor* DamageCauser)
{
    GetCharacterMovement()->AddImpulse(ShotFromDirection * KnockbackStrength, true);
}
