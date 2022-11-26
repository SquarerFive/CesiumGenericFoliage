// Fill out your copyright notice in the Description page of Project Settings.


#include "CesiumFoliageActor.h"

#include "Cesium3DTileset.h"
#include "CesiumGeoreference.h"
#include "CesiumGlobeAnchorComponent.h"
//#include "Actors/Components/FoliageCaptureComponent.h"

#include "EngineUtils.h"

ACesiumFoliageActor::ACesiumFoliageActor() :AGenericFoliageActor()
{
	GlobeAnchorComponent = CreateDefaultSubobject<UCesiumGlobeAnchorComponent>("GlobeAnchor");
}

void ACesiumFoliageActor::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	TArray<AActor*> ShowOnlyActors;
	
	for (TActorIterator<ACesium3DTileset> TilesetIter(GetWorld()); TilesetIter; ++TilesetIter)
	{
		if (*TilesetIter)
		{
			ShowOnlyActors.Add(*TilesetIter);
		}
	}

	if (ShowOnlyActors.Num() > 0)
	{
		SetShowOnlyActors(ShowOnlyActors);
	}
}

void ACesiumFoliageActor::BeginPlay()
{
	Super::BeginPlay();
}

FRotator ACesiumFoliageActor::CalculateEastNorthUp(const FVector& InWorldLocation) const
{
	ACesiumGeoreference* Georeference = GlobeAnchorComponent->ResolveGeoreference();
	if (Georeference != nullptr)
	{
		return Georeference->ComputeEastSouthUpToUnreal(InWorldLocation).Rotator();
	}

	return FRotator::ZeroRotator;
}

FVector ACesiumFoliageActor::WorldToLocalPosition(const FVector& InWorldLocation) const
{
	ACesiumGeoreference* Georeference = GlobeAnchorComponent->ResolveGeoreference();
	if (Georeference != nullptr)
	{
		return Georeference->TransformUnrealToLongitudeLatitudeHeight(InWorldLocation);
	}

	return FVector::ZeroVector;
}

FVector ACesiumFoliageActor::LocalToWorldPosition(const FVector& InLocalLocation) const
{
	ACesiumGeoreference* Georeference = GlobeAnchorComponent->ResolveGeoreference();
	if (Georeference != nullptr)
	{
		return Georeference->TransformLongitudeLatitudeHeightToUnreal(InLocalLocation);
	}

	return FVector::ZeroVector;
}

FVector ACesiumFoliageActor::AdjustWorldPositionHeightToPlanet(const FVector& InWorldLocation,
	const double& Height) const
{
	FHitResult Result;
	FVector LocalPosition;
	
	// World centre
	const FVector Target = GlobeAnchorComponent->ResolveGeoreference()->TransformEcefToUnreal(FVector::ZeroVector);
	const FQuat ENU = GlobeAnchorComponent->ResolveGeoreference()->ComputeEastSouthUpToUnreal(InWorldLocation).ToQuat();
	
	if (GetWorld()->LineTraceSingleByChannel(Result, InWorldLocation + (ENU.GetUpVector() * 4000 * 100), Target, ECollisionChannel::ECC_Visibility))
	{
		if (Result.bBlockingHit)
		{
			LocalPosition = WorldToLocalPosition(Result.Location);
		}
	} else
	{
		LocalPosition = WorldToLocalPosition(InWorldLocation);
		LocalPosition.Z = 0;
	}

	LocalPosition.Z = LocalPosition.Z + Height;
	
	return LocalToWorldPosition(LocalPosition);
}
