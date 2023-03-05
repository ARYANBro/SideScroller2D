// Fill out your copyright notice in the Description page of Project Settings.


#include "PaperPlayer.h"

#include <Camera/CameraComponent.h>
#include <GameFramework/SpringArmComponent.h>
#include <GameFramework/CharacterMovementComponent.h>
#include <Engine/DamageEvents.h>
    
APaperPlayer::APaperPlayer()
{
}

void APaperPlayer::BeginPlay()
{
    
    Super::BeginPlay();

    JetpackDuration = MaxJetpackDuration;

    if (CameraClass)
    {
        Camera = GetWorld()->SpawnActor(CameraClass);
        ensure(Camera);
    }
    else
        UE_LOG(LogTemp, Error, TEXT("Camera class is null"));


    if (auto PlayerController = Cast<APlayerController>(GetController()))
        PlayerController->SetViewTarget(Camera);
    else
        UE_LOG(LogTemp, Error, TEXT("Player Controller is null or couldn't cast to APlayerController"));

    FScriptDelegate TakeRadialDamageDelegate;
    TakeRadialDamageDelegate.BindUFunction(this, "TakeRadialDamage");
    OnTakeRadialDamage.Add(TakeRadialDamageDelegate);
}

void APaperPlayer::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (Camera)
        Camera->SetActorLocation({ GetActorLocation().X, GetActorLocation().Y, Camera->GetActorLocation().Z });
    else
        UE_LOG(LogTemp, Error, TEXT("Camera is null"));

    UpdateJetpack(DeltaTime);
    AddMovementInput(GetActorForwardVector(), 1.0);
}

void APaperPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    if (PlayerInputComponent)
    {
        PlayerInputComponent->BindAction("Jump", EInputEvent::IE_Pressed, this, &APaperPlayer::Jump);
        PlayerInputComponent->BindAction("Jump", EInputEvent::IE_Released, this, &APaperPlayer::StopJumping);
        PlayerInputComponent->BindAxis("MoveForwardBackward", this, &APaperPlayer::MoveForwardBackward);
    }
}

void APaperPlayer::UpdateJetpack(float DeltaTime)
{
    if (CanUpdateJetpack() && bJetpackActive)
    {
        LaunchCharacter({ 0.0, 0.0, LaunchZVelocity }, false, true);
        JetpackDuration -= DeltaTime;
    }

    if (JetpackDuration <= 0.0)
    {
        bJetpackActive = false;
    }
}

void APaperPlayer::MoveForwardBackward(float AxisValue)
{
    if (bEnableMovement)
        AddMovementInput(GetActorForwardVector(), AxisValue);
}

void APaperPlayer::Jump()
{
    if (auto MovementComponent = GetCharacterMovement(); MovementComponent->IsFalling() && CanUpdateJetpack())
    {
        bJetpackActive = true;
        OriginalAirControl = MovementComponent->AirControl;
        MovementComponent->AirControl = JetpackAirControl;
    }
    else if (GetCharacterMovement()->IsWalking())
    {
        APaperCharacter::Jump();
    }
}

void APaperPlayer::StopJumping()
{
    APaperCharacter::StopJumping();
    bJetpackActive = false;
    GetCharacterMovement()->AirControl = OriginalAirControl;
}

void APaperPlayer::Landed(const FHitResult& HitResult)
{
    APaperCharacter::Landed(HitResult);
    JetpackDuration = MaxJetpackDuration;
}

float APaperPlayer::TakeDamage(float DamageTaken, const FDamageEvent& DamageEvent, AController* EventInstigator, AActor* Causer)
{
    float Damage = Super::TakeDamage(DamageTaken, DamageEvent, EventInstigator, Causer);
    Damage = FMath::Min(Damage, Health);
    Health -= Damage;
    if (Health <= 0)
        Destroy();
    return Damage;
}

void APaperPlayer::TakeRadialDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, FVector Origin, FHitResult HitInfo, AController* InstigatedBy, AActor* DamageCauser)
{
    Health -= FMath::Min(Damage, Health);
    FVector RotatedVector = GetActorForwardVector().RotateAngleAxis(-135.0f, FVector(0.0f, 1.0f, 0.0f));
    GetCharacterMovement()->AddImpulse(RotatedVector * KnockbackStrength, true);
}
