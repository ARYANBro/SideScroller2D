// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Jetpack.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SIDESCROLLER2D_API UJetpack : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UJetpack();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void StartJetpack();
	void StopJetpack();
	void Reset();
	bool CanUpdate() { return RemainingJetpackDuration > 0.0f; }
	bool IsEnabled() { return bEnabled; }

private:
	UPROPERTY(EditAnywhere, Category = "Jetpack")
	float LaunchZVelocity;

	UPROPERTY(EditAnywhere, Category = "Jetpack")
	float AirControl;

	UPROPERTY(EditAnywhere, Category = "Jetpack")
	float MaxJetpackDuration;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Jetpack", meta = (AllowPrivateAccess))
	bool bEnabled = false;

	float OriginalAirControl;
	class ACharacter* Owner = nullptr;
	float RemainingJetpackDuration = 0.0f;
};
