// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperCharacter.h"
#include "PaperPlayer.generated.h"

/**
 * 
 */
UCLASS()
class SIDESCROLLER2D_API APaperPlayer : public APaperCharacter
{
	GENERATED_BODY()

public:
	APaperPlayer();

	void BeginPlay();
	void Tick(float DeltaTime);

	bool CanUpdateJetpack() { return JetpackDuration > 0.0; }

private:
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	void UpdateJetpack(float DeltaTime);
	void MoveForwardBackward(float AxisValue);
	virtual void Jump() override;
	virtual void StopJumping() override;
	virtual void Landed(const FHitResult& HitResult) override;
	
	virtual float TakeDamage(float DamageTaken, const struct FDamageEvent& DamageEvent, AController* EventInstigator, AActor* Causer) override;

	UFUNCTION()
	void TakeRadialDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, FVector Origin, FHitResult HitInfo, class AController* InstigatedBy, AActor* DamageCauser);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PaperPlayer")
	bool bEnableMovement;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PaperPlayer")
	bool bJetpackActive = false;

private:
	UPROPERTY(EditDefaultsOnly, Category = "PaperPlayer")
	TSubclassOf<AActor> CameraClass;

	UPROPERTY(VisibleAnywhere, Category = "PaperPlayer")
	AActor* Camera;

	UPROPERTY(EditDefaultsOnly, Category = "PaperPlayer")
	float LaunchZVelocity;

	UPROPERTY(EditDefaultsOnly, Category = "PaperPlayer")
	float MaxJetpackDuration;

	UPROPERTY(EditDefaultsOnly, Category = "PaperPlayer", meta = (ClampMin = "0.0", ClampMax = "1.0"))
	float JetpackAirControl;

	UPROPERTY(EditDefaultsOnly, Category = "PaperPlayer")
	float KnockbackStrength = 650.0f;

	UPROPERTY(EditDefaultsOnly, Category = "PaperPlayer")
	float Health = 100.0f;

	float JetpackDuration;
	float OriginalAirControl;
	FVector OriginalCameraLocation;
};
