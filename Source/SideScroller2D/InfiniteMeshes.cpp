// Fill out your copyright notice in the Description page of Project Settings.

#include "InfiniteMeshes.h"

#include <Kismet/GameplayStatics.h>
#include <Kismet/KismetMathLibrary.h>

// Sets default values
AInfiniteMeshes::AInfiniteMeshes()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Spline = CreateDefaultSubobject<USplineComponent>(TEXT("Spline"));
	RootComponent = Spline;
}

// Called when the game starts or when spawned
void AInfiniteMeshes::BeginPlay()
{
	Super::BeginPlay();
}

void AInfiniteMeshes::Destroyed()
{
	DeleteMeshes();
}

void AInfiniteMeshes::SpawnMeshes()
{
	if (!GetWorld())
	{
		UE_LOG(LogTemp, Error, TEXT("void AInfiniteBuildings::SpawnMeshes(): World is null"));
		return;
	}

	int NumOfMeshes = Spline->GetNumberOfSplinePoints() - 1;
	for (int i = 0; i < NumOfMeshes; i++)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.bAllowDuringConstructionScript = true;
		auto StaticMeshActor = GetWorld()->SpawnActor<AStaticMeshActor>(SpawnParams);
		StaticMeshActor->Tags.Add(FName(FString("Infinite Buildings ") + GetName()));
		StaticMeshActor->SetMobility(EComponentMobility::Movable);
		
#if WITH_EDITOR
		StaticMeshActor->SetFolderPath(GetFolderPath());
#endif
		StaticMeshActor->SetActorLocation(Spline->GetLocationAtSplinePoint(i, ESplineCoordinateSpace::World) + Transform.GetLocation());
		StaticMeshActor->SetActorScale3D(Transform.GetScale3D());
		StaticMeshActor->SetActorRotation(Transform.GetRotation());

		if (Meshes.Num() > 0)
			StaticMeshActor->GetStaticMeshComponent()->SetStaticMesh(Meshes[UKismetMathLibrary::RandomIntegerInRange(0, Meshes.Num() - 1)]);
		else
			UE_LOG(LogTemp, Warning, TEXT("Not Elements in the \"Meshes\" array"));
	}
}

void AInfiniteMeshes::DeleteMeshes()
{
	TArray<AActor*> MeshActors;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName(FString("Infinite Buildings ") + GetName()), MeshActors);
	for (auto MeshActor : MeshActors)
	{
		if (MeshActor)
			MeshActor->Destroy();
	}
}

// Called every frame
void AInfiniteMeshes::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector LastSplinePoint = GetLastSplinePointLocation(ESplineCoordinateSpace::World);
	FVector FirstSplinePoint = GetFirstSplinePointLocation(ESplineCoordinateSpace::World);
	FVector LastSecondSplinePoint = Spline->GetLocationAtSplinePoint(FMath::Max(Spline->GetNumberOfSplinePoints() - 2, 0), ESplineCoordinateSpace::World);

	FVector2D LastSplinePointOnScreen;
	FVector2D LastSecondSplinePointOnScreen;
	FVector2D ViewportSize;
	GetWorld()->GetGameViewport()->GetViewportSize(ViewportSize);
	UGameplayStatics::ProjectWorldToScreen(GetWorld()->GetFirstPlayerController(), LastSplinePoint, LastSplinePointOnScreen);
	UGameplayStatics::ProjectWorldToScreen(GetWorld()->GetFirstPlayerController(), LastSecondSplinePoint, LastSecondSplinePointOnScreen);

	float ScreenDistBetweenPoints = FVector2D::Distance(LastSplinePointOnScreen, LastSecondSplinePointOnScreen);	

	if (LastSplinePointOnScreen.X < 0.0 - ScreenDistBetweenPoints)
		Destroy();
	else if (FMath::Abs(LastSplinePointOnScreen.X - ViewportSize.X)  < ScreenDistBetweenPoints && !bSpawnedNext)
	{
		FVector Loc = LastSplinePoint;
		auto NextActor = GetWorld()->SpawnActor(GetClass(), &Loc);
		check(NextActor);
		bSpawnedNext = true;
	}
}

void AInfiniteMeshes::OnConstruction(const FTransform& Trans)
{
	TArray<FVector> SplinePoints;
	for (int i = 0; i < NumPoints; i++)
	{
		SplinePoints.Add({ i * DistBetweenPoints, 0.0, 0.0 });
	}

	Spline->SetSplinePoints(SplinePoints, ESplineCoordinateSpace::Local);
	DeleteMeshes();
	SpawnMeshes();
}
