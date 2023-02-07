// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <Components/SplineComponent.h>
#include <Engine/StaticMeshActor.h>
#include "InfiniteMeshes.generated.h"

UCLASS()
class SIDESCROLLER2D_API AInfiniteMeshes : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AInfiniteMeshes();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void OnConstruction(const FTransform& Transform);
	USplineComponent* GetSplineComponent() { return Spline; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void Destroyed() override;
	void SetNumberOfPoints(int Count) { NumPoints = Count; }
	void SetDistanceBetweenPoints(int Distance) { DistBetweenPoints = Distance; }
   	FVector GetLastSplinePointLocation(ESplineCoordinateSpace::Type SplineCoordinateSpace)  { return Spline->GetLocationAtSplinePoint(Spline->GetNumberOfSplinePoints() - 1, SplineCoordinateSpace); }
	FVector GetFirstSplinePointLocation(ESplineCoordinateSpace::Type SplineCoordinateSpace)  { return Spline->GetLocationAtSplinePoint(0, SplineCoordinateSpace); }

private:
	void SpawnMeshes();
	void DeleteMeshes();

private:
	UPROPERTY(VisibleAnywhere)
	USplineComponent* Spline;

	UPROPERTY(EditAnywhere, Category="InfiniteBuildings")
	int NumPoints;

	UPROPERTY(EditAnywhere, Category="InfiniteBuildings")
	float DistBetweenPoints;

	UPROPERTY(EditAnywhere, Category="InfiniteBuildings")
	TArray<UStaticMesh*> Meshes;

	UPROPERTY(EditAnywhere, Category="InfiniteBuildings")
	FTransform Transform;

	bool bSpawnedNext = false;
};
