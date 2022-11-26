// Fill out your copyright notice in the Description page of Project Settings.


#include "CesiumClusterFoliageActor.h"
#include "EngineUtils.h"

void ACesiumClusterFoliageActor::BeginPlay()
{
	Super::BeginPlay();

	for (TActorIterator<ACesiumGeoreference> ActorIter(GetWorld()); ActorIter; ++ActorIter)
	{
		if (IsValid(*ActorIter))
		{
			GeoReference = *ActorIter;
			break;
		}
	}

	for (TActorIterator<ACesium3DTileset> ActorIter(GetWorld()); ActorIter; ++ActorIter)
	{
		Cesium3DTileset = *ActorIter;
	}
}

FVector ACesiumClusterFoliageActor::GeographicToEngineLocation(const FVector& GeographicLocation)
{
	return GeoReference->TransformLongitudeLatitudeHeightToUnreal(GeographicLocation);
}

FVector ACesiumClusterFoliageActor::EngineToGeographicLocation(const FVector& EngineLocation)
{
	return GeoReference->TransformUnrealToLongitudeLatitudeHeight(EngineLocation);
}

FVector ACesiumClusterFoliageActor::GetUpVectorFromGeographicLocation(const FVector& GeographicLocation)
{
	return GeoReference->ComputeEastSouthUpToUnreal(GeographicToEngineLocation(GeographicLocation)).ToQuat().GetUpVector();
}

FVector ACesiumClusterFoliageActor::GetUpVectorFromEngineLocation(const FVector& EngineLocation)
{
	return GeoReference->ComputeEastSouthUpToUnreal(EngineLocation).ToQuat().GetUpVector();
}

double ACesiumClusterFoliageActor::GetTerrainBaseHeight(const FVector& GeographicLocation, FVector& OutNormal)
{
	TArray<FHitResult> Results;
	const FVector StartLocation = GeographicToEngineLocation(
		FVector(GeographicLocation.X, GeographicLocation.Y, 9000.0));
	const FVector EndLocation =
		GeographicToEngineLocation(FVector(GeographicLocation.X, GeographicLocation.Y, -1000.0));
	if (GetWorld()->LineTraceMultiByChannel(Results, StartLocation, EndLocation, ECollisionChannel::ECC_Visibility))
	{
		const TArray<FHitResult> HitTerrain = Results.FilterByPredicate([this](const FHitResult& Result)
		{
			return Result.GetActor() == Cesium3DTileset; 
		});

		if (HitTerrain.Num() > 0)
		{
			OutNormal = HitTerrain[0].ImpactNormal;
			return EngineToGeographicLocation(HitTerrain[0].Location).Z;
		}
	}

	return 0.0;
}
