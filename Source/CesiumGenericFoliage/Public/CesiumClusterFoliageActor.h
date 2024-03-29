// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Cesium3DTileset.h"
#include "CesiumGeoreference.h"
#include "Actors/ClusterFoliageActor.h"
#include "CesiumClusterFoliageActor.generated.h"

/**
 * 
 */
UCLASS()
class CESIUMGENERICFOLIAGE_API ACesiumClusterFoliageActor : public AClusterFoliageActor
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;
	virtual FVector GeographicToEngineLocation(const FVector& GeographicLocation) override;
	virtual FVector EngineToGeographicLocation(const FVector& EngineLocation) override;
	virtual FVector GetUpVectorFromGeographicLocation(const FVector& GeographicLocation) override;
	virtual FVector GetUpVectorFromEngineLocation(const FVector& EngineLocation) override;
	virtual double GetTerrainBaseHeight(const FVector& GeographicLocation, FVector& OutNormal) override;
private:
	UPROPERTY()
	ACesiumGeoreference* GeoReference = nullptr;
	UPROPERTY()
	ACesium3DTileset* Cesium3DTileset = nullptr;
};
