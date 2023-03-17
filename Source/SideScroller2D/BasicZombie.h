// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperCharacter.h"
#include "BasicZombie.generated.h"

/**
 * 
 */
class UPrimitveComponent;

UCLASS()
class SIDESCROLLER2D_API ABasicZombie : public APaperCharacter
{
	GENERATED_BODY()

public:
	void BeginPlay();
	void Tick(float DeltaTime);

	virtual float TakeDamage(float Damage, const FDamageEvent& DamageEvent, AController* EventInstigator, AActor* Causer) override;
protected:
	UFUNCTION()
	void OnCapsuleBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void TakePointDamage(AActor* DamagedActor, float Damage, class AController* InstigatedBy, FVector HitLocation, class UPrimitiveComponent* FHitComponent, FName BoneName, FVector ShotFromDirection, const class UDamageType* DamageType, AActor* DamageCauser);

private:
	UPROPERTY(EditAnywhere, Category = "BasicZombie")
	bool bEnableMovement = true;

	UPROPERTY(EditDefaultsOnly, Category = "BasiZombie")
	float Health = 100.0f;

	UPROPERTY(EditDefaultsOnly, Category = "BasicZombie")
	float DamageInflicted = 10.0f;

	UPROPERTY(EditDefaultsOnly, Category = "BasicZombie")
	float KnockbackStrength = 1500.0f;
};
