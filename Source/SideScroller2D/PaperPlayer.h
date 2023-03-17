// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperCharacter.h"
#include "PaperPlayer.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
namespace EAdditionalMovement
{
	enum Type
	{
		None UMETA(DisplayName = "None"),
		UsingJetpack UMETA(DisplayName = "UsingJetpack"),
		SplashingDown UMETA(DisplayName = "SplashingDown")

	};
}

UCLASS()
class SIDESCROLLER2D_API APaperPlayer : public APaperCharacter
{
	GENERATED_BODY()

public:
	APaperPlayer();

	void BeginPlay();
	void Tick(float DeltaTime);

private:
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	void MoveForwardBackward(float AxisValue);
	virtual void Jump() override;
	virtual void StopJumping() override;

	UFUNCTION()
	void Grounded(const FHitResult& HitResult);

	void SplashDown();
	
	virtual float TakeDamage(float DamageTaken, const struct FDamageEvent& DamageEvent, AController* EventInstigator, AActor* Causer) override;

	UFUNCTION()
	void TakeAnyDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

	bool CanTakeDamage() const { return !bTookDamage; }
	bool CanSplashDown() const { return !bDidSplashDown; }

protected:
	UFUNCTION(BlueprintNativeEvent)
	void SplashDownGrounded();

private:
	virtual void SplashDownGrounded_Implementation();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PaperPlayer")
	bool bEnableMovement;

	UPROPERTY(BlueprintReadOnly, Category = "PaperPlayer")
	TEnumAsByte<EAdditionalMovement::Type> AdditionalMovement = EAdditionalMovement::None;

private:
	UPROPERTY(EditAnywhere, Category = "Camera")
	TSubclassOf<class UCameraShakeBase> CameraShake;

	UPROPERTY(EditAnywhere, Category = "Camera")
	TSubclassOf<AActor> CameraClass;

	UPROPERTY(VisibleAnywhere, Category = "Camera")
	AActor* Camera;

	UPROPERTY(EditAnywhere, Category = "PaperPlayer")
	float KnockbackStrength = 650.0f;

	UPROPERTY(EditAnywhere, Category = "PaperPlayer")
	float Health = 100.0f;

	UPROPERTY(EditAnywhere, Category = "PaperPlayer")
	FVector PlayerKnockbackRotateAxis = FVector(0.0f, 1.0f, 0.0f);

	UPROPERTY(EditAnywhere, Category = "PaperPlayer")
	float PlayerKnockbackAngle = -135.0f;

	UPROPERTY(EditAnywhere, Category = "Abilities")
	float SplashRadius = 350.0f;

	UPROPERTY(EditAnywhere, Category = "Abilities")
	float DamageInflicted = 100.0f;

	UPROPERTY(EditAnywhere, Category = "PaperPlayer")
	float SplashZVelocity = -1200.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess))
	class UJetpack* Jetpack = nullptr;

	UPROPERTY(EditAnywhere, Category = "PaperPlayer")
	float DamageCooldown = 2.0f;

	UPROPERTY(EditAnywhere, Category = "PaperPlayer")
	float SplashDownCooldown = 1.0f;

	UPROPERTY(EditAnywhere, Category = "PaperPlayer")
	float SplashDownResetCooldown = 0.5f;

	UPROPERTY(EditAnywhere, Category = "PaperPlayer")
	FName PlayerRunCollisionProfile;

	UPROPERTY(EditAnywhere, Category = "PaperPlayer")
	FName PlayerSplashDownCollisionProfile;

	float CurrentDamageCooldown;
	float CurrentSplashDownCooldown;
	bool bTookDamage = false;
	bool bDidSplashDown = false;
};
