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
	return GeoReference->ComputeEastNorthUpToUnreal(GeographicToEngineLocation(GeographicLocation)).ToQuat().GetUpVector();
}

FVector ACesiumClusterFoliageActor::GetUpVectorFromEngineLocation(const FVector& EngineLocation)
{
	return GeoReference->ComputeEastNorthUpToUnreal(EngineLocation).ToQuat().GetUpVector();
}
