// Fill out your copyright notice in the Description page of Project Settings.


#include "PaperPlayer.h"

#include <Camera/CameraComponent.h>
#include <GameFramework/SpringArmComponent.h>
#include <GameFramework/CharacterMovementComponent.h>

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
    if (bJetpackActive)
    {
        LaunchCharacter({ 0.0, 0.0, LaunchZVelocity }, false, true);
        JetpackDuration -= DeltaTime;
    }

    if (JetpackDuration <= 0.0)
    {
        bJetpackActive = false;
        JetpackDuration = MaxJetpackDuration;
    }
}

void APaperPlayer::MoveForwardBackward(float AxisValue)
{
    if (bEnableMovement)
        AddMovementInput(GetActorForwardVector(), AxisValue);
}

void APaperPlayer::Jump()
{
    if (auto MovementComponent = GetCharacterMovement(); MovementComponent->IsFalling())
    {
        bJetpackActive = true;
        OriginalAirControl = MovementComponent->AirControl;
        MovementComponent->AirControl = JetpackAirControl;
    }
    else
        APaperCharacter::Jump();
}

void APaperPlayer::StopJumping()
{
    APaperCharacter::StopJumping();
    bJetpackActive = false;
    GetCharacterMovement()->AirControl = OriginalAirControl;
}
