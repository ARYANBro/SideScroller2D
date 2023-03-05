// Fill out your copyright notice in the Description page of Project Settings.


#include "BasicZombie.h"

#include <Components/ArrowComponent.h>
#include <Components/CapsuleComponent.h>
#include <Engine/DamageEvents.h>

void ABasicZombie::BeginPlay()
{
    Super::BeginPlay();

    FScriptDelegate Delegate;
    Delegate.BindUFunction(this, "OnCapsuleHit");
    GetCapsuleComponent()->OnComponentHit.Add(Delegate);
}

void ABasicZombie::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    if (bEnableMovement)
        AddMovementInput(GetArrowComponent()->GetForwardVector(), 1.0f);
}

void ABasicZombie::OnCapsuleHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
    if (OtherActor->CanBeDamaged())
    {
        FRadialDamageEvent DamageEvent;
        DamageEvent.Params.BaseDamage = 10;
        DamageEvent.Origin = GetActorLocation();
        DamageEvent.Params.DamageFalloff = 0;
        DamageEvent.Params.InnerRadius = 20;
        DamageEvent.Params.OuterRadius = 20;
        DamageEvent.Params.MinimumDamage = 10;

        OtherActor->TakeDamage(DamageEvent.Params.BaseDamage, DamageEvent, GetController(), this);
        UE_LOG(LogTemp, Log, TEXT("Damaged Actor"));
    }
}
