// Fill out your copyright notice in the Description page of Project Settings.
#pragma once
#include "CoreMinimal.h"
#include "Spline.h"
#include "Spline_Spawn.generated.h"

/**
 * 
 */
UCLASS()
class TOWER_API ASpline_Spawn : public ASpline
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere)
		bool bRaycastToGround = true;

	// Spawn an actor on the spline at the given Alpha position (0->1) where 0 is the start
	// of the spline and 1 is the end
	UFUNCTION(BlueprintCallable, Category = "Spline")
		void Spawn(TSubclassOf<AActor> Class, float Alpha);

private:

	const float RayCastUpDist = 500.0f;
	const float RayCastDownDist = RayCastUpDist * 2.0f;
	const ECollisionChannel RayCastCollectionChannel = ECollisionChannel::ECC_WorldStatic;

};
