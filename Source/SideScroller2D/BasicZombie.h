// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperCharacter.h"
#include "BasicZombie.generated.h"

/**
 * 
 */
UCLASS()
class SIDESCROLLER2D_API ABasicZombie : public APaperCharacter
{
	GENERATED_BODY()

public:
	void BeginPlay();
	void Tick(float DeltaTime);

protected:
	UFUNCTION()
	void OnCapsuleHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

private:
	UPROPERTY(EditAnywhere, Category = "BasicZombie")
	bool bEnableMovement = true;
};
