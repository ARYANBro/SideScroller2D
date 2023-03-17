// Fill out your copyright notice in the Description page of Project Settings.


#include "PaperPlayer.h"

#include "BasicZombie.h"
#include "Jetpack.h"

#include <Components/CapsuleComponent.h>
#include <Camera/CameraComponent.h>
#include <Camera/CameraShakeBase.h>
#include <GameFramework/SpringArmComponent.h>
#include <GameFramework/CharacterMovementComponent.h>
#include <Engine/DamageEvents.h>
#include <Kismet/GameplayStatics.h>
    
APaperPlayer::APaperPlayer()
{
    Jetpack = CreateDefaultSubobject<UJetpack>(TEXT("PlayerJetpack"));
    Jetpack->SetComponentTickEnabled(false);
    AddOwnedComponent(Jetpack);   
}

void APaperPlayer::BeginPlay()
{
    Super::BeginPlay();

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

    FScriptDelegate OnTakeAnyDamageDelegate;
    OnTakeAnyDamageDelegate.BindUFunction(this, "TakeAnyDamage");
    OnTakeAnyDamage.Add(OnTakeAnyDamageDelegate);

    FScriptDelegate OnLandedDelegate;
    OnLandedDelegate.BindUFunction(this, "Grounded");
    LandedDelegate.Add(OnLandedDelegate);

    CurrentDamageCooldown = DamageCooldown;
    CurrentSplashDownCooldown = SplashDownCooldown;
}

void APaperPlayer::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (Camera)
        Camera->SetActorLocation({ GetActorLocation().X, GetActorLocation().Y, Camera->GetActorLocation().Z });
    else
        UE_LOG(LogTemp, Error, TEXT("Camera is null"));


    switch (AdditionalMovement)
    {
        case EAdditionalMovement::UsingJetpack:
            if (!Jetpack->IsComponentTickEnabled())
                Jetpack->SetComponentTickEnabled(true);
            break;

        case EAdditionalMovement::SplashingDown:    
            if (Jetpack->IsComponentTickEnabled())
                Jetpack->SetComponentTickEnabled(false);

            if (CanSplashDown())
            {
                LaunchCharacter({ 0.0f, 0.0f, SplashZVelocity }, false, true);
                bDidSplashDown = true;
                GetCapsuleComponent()->SetCollisionProfileName(PlayerSplashDownCollisionProfile);
                UE_LOG(LogTemp, Log, TEXT("Splashed Down"));
            }

            break;
    
    default:
        break;
    }

    AddMovementInput(GetActorForwardVector(), 1.0);

    if (bTookDamage)
    {
        CurrentDamageCooldown -= DeltaTime;
        if (CurrentDamageCooldown <= 0)
        {
            CurrentDamageCooldown = DamageCooldown;
            bTookDamage = false;
        }
    }

    if (bDidSplashDown)
    {
        CurrentSplashDownCooldown -= DeltaTime;
        if (CurrentSplashDownCooldown <= 0)
        {
            CurrentSplashDownCooldown = SplashDownCooldown;
            bDidSplashDown = false;
        }
    }
}

void APaperPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    if (PlayerInputComponent)
    {
        PlayerInputComponent->BindAction("Jump", EInputEvent::IE_Pressed, this, &APaperPlayer::Jump);
        PlayerInputComponent->BindAction("Jump", EInputEvent::IE_Released, this, &APaperPlayer::StopJumping);
        PlayerInputComponent->BindAction("SplashDown", EInputEvent::IE_Pressed, this, &APaperPlayer::SplashDown);
        PlayerInputComponent->BindAxis("MoveForwardBackward", this, &APaperPlayer::MoveForwardBackward);
    }
}

void APaperPlayer::MoveForwardBackward(float AxisValue)
{
    if (bEnableMovement)
        AddMovementInput(GetActorForwardVector(), AxisValue);
}

void APaperPlayer::Jump()
{
    if (auto MovementComponent = GetCharacterMovement(); MovementComponent->IsFalling() && AdditionalMovement == EAdditionalMovement::None)
    {
        AdditionalMovement = EAdditionalMovement::UsingJetpack;
        Jetpack->StartJetpack();
    }
    else if (GetCharacterMovement()->IsWalking())
        APaperCharacter::Jump();
}

void APaperPlayer::StopJumping()
{
    APaperCharacter::StopJumping();
    Jetpack->StopJetpack();
}

void APaperPlayer::Grounded(const FHitResult& HitResult)
{
    if (AdditionalMovement == EAdditionalMovement::SplashingDown)
        SplashDownGrounded();

    Jetpack->Reset();
    AdditionalMovement = EAdditionalMovement::None;
}

void APaperPlayer::SplashDown()
{
    if (GetCharacterMovement()->IsFalling() && AdditionalMovement == EAdditionalMovement::UsingJetpack && CanSplashDown())
    {
        AdditionalMovement = EAdditionalMovement::SplashingDown;
    }
}

float APaperPlayer::TakeDamage(float DamageTaken, const FDamageEvent& DamageEvent, AController* EventInstigator, AActor* Causer)
{
    float Damage = Super::TakeDamage(DamageTaken, DamageEvent, EventInstigator, Causer);
    Damage = FMath::Min(Damage, Health);
    Health -= Damage;
    if (Health <= 0)
    {
        // TODO: Make an OnDeath BlueprintNativeEvent
        Destroy();
    }
    
    return Damage;
}

void APaperPlayer::TakeAnyDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser)
{
    if (CanTakeDamage())
    {
        UE_LOG(LogTemp, Log, TEXT("Took Damage"));
        Health -= FMath::Min(Damage, Health);
        FVector RotatedVector = GetActorForwardVector().RotateAngleAxis(PlayerKnockbackAngle, PlayerKnockbackRotateAxis);
        GetCharacterMovement()->AddImpulse(RotatedVector * KnockbackStrength, true);
        bTookDamage = true;
    }
}

void APaperPlayer::SplashDownGrounded_Implementation()
{
    if (auto PlayerController = Cast<APlayerController>(GetController()))
        PlayerController->ClientStartCameraShake(CameraShake);
    else
        UE_LOG(LogTemp, Warning, TEXT("void APaperPlayer::SplashDown | PlayerController is nullptr"));

    TArray<FHitResult> OutHits;
    GetWorld()->SweepMultiByProfile(OutHits, GetActorLocation(), GetActorLocation(), FQuat::Identity, "Zombie_Trace", FCollisionShape::MakeSphere(SplashRadius));
    for (const FHitResult& Hit : OutHits)
    {
        if (auto BasicZombie = Cast<ABasicZombie>(Hit.GetActor()); BasicZombie)
        {
            FVector Direction = FVector(0.0f, 0.0f, 1.0f);
            Direction += (BasicZombie->GetActorLocation() - GetActorLocation()).GetSafeNormal2D();
            Direction.Normalize();

            FPointDamageEvent DamageEvent;
            DamageEvent.HitInfo = Hit;
            DamageEvent.Damage = DamageInflicted;
            DamageEvent.ShotDirection = Direction;
            BasicZombie->TakeDamage(DamageInflicted, DamageEvent, GetController(), this);
        }
    }   

    FTimerHandle TH;
    TFunction<void ()> Callback = [this]()
    {
        GetCapsuleComponent()->SetCollisionProfileName(PlayerRunCollisionProfile);
    };

    GetWorld()->GetTimerManager().SetTimer(TH, std::move(Callback), SplashDownResetCooldown, false);
}
