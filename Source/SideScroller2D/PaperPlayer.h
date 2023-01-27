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

private:
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	void UpdateJetpack(float DeltaTime);
	void MoveForwardBackward(float AxisValue);
	virtual void Jump() override;
	virtual void StopJumping() override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PaperPlayer")
	bool bEnableMovement;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PaperPlayer")
	bool bJetpackActive = false;

private:
	UPROPERTY(VisibleAnywhere)
	class UCameraComponent* Camera;

	UPROPERTY(VisibleAnywhere)
	class USpringArmComponent* SpringArm;

	UPROPERTY(EditDefaultsOnly, Category = "PaperPlayer")
	float LaunchZVelocity;

	UPROPERTY(EditDefaultsOnly, Category = "PaperPlayer")
	float MaxJetpackDuration;

	UPROPERTY(EditDefaultsOnly, Category = "PaperPlayer", meta = (ClampMin = "0.0", ClampMax = "1.0"))
	float JetpackAirControl;

	float JetpackDuration;
	float OriginalAirControl;
};
