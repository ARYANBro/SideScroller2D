// Fill out your copyright notice in the Description page of Project Settings.

#include "Jetpack.h"
#include <GameFramework/Character.h>
#include <GameFramework/CharacterMovementComponent.h>

// Sets default values for this component's properties
UJetpack::UJetpack()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	// ...
}

// Called when the game starts
void UJetpack::BeginPlay()
{
	Super::BeginPlay();

	// ...
	Owner = Cast<ACharacter>(GetOwner());
	check(Owner);
	OriginalAirControl = Owner->GetCharacterMovement()->AirControl;
	RemainingJetpackDuration = MaxJetpackDuration;
}

// Called every frame
void UJetpack::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bEnabled && CanUpdate())
	{
		RemainingJetpackDuration -= DeltaTime;
		Owner->LaunchCharacter({ 0.0f, 0.0f, LaunchZVelocity }, false, true);
	}
	
	if (RemainingJetpackDuration <= 0.0f)
		StopJetpack();

	// ...
}

void UJetpack::StartJetpack()
{
	if (CanUpdate())
	{
		bEnabled = true;
		Owner->GetCharacterMovement()->AirControl = AirControl;
	}
}

void UJetpack::StopJetpack()
{
	bEnabled = false;
	Owner->GetCharacterMovement()->AirControl = OriginalAirControl;
}

void UJetpack::Reset()
{
	RemainingJetpackDuration = MaxJetpackDuration;
}

