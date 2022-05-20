// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/GenericFoliageActor.h"
#include "CesiumFoliageActor.generated.h"

/**
 * 
 */
UCLASS()
class CESIUMGENERICFOLIAGE_API ACesiumFoliageActor : public AGenericFoliageActor
{
	GENERATED_BODY()

public:
	ACesiumFoliageActor();

	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void BeginPlay() override;
protected:
	UPROPERTY(VisibleDefaultsOnly, Category = "Components")
		class UCesiumGlobeAnchorComponent* GlobeAnchorComponent;
public:
	virtual FRotator CalculateEastNorthUp(const FVector& InWorldLocation) const override;
	virtual FVector WorldToLocalPosition(const FVector& InWorldLocation) const override;
	virtual FVector LocalToWorldPosition(const FVector& InLocalLocation) const override;

	virtual FVector AdjustWorldPositionHeightToPlanet(const FVector& InWorldLocation, const double& Height) const override;
};
