// Fill out your copyright notice in the Description page of Project Settings.
#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Spline.generated.h"

class USplineComponent;

UCLASS()
class TOWER_API ASpline : public AActor
{
	GENERATED_BODY()
	
public:	

	ASpline();

	UFUNCTION(BlueprintPure)
		USplineComponent* GetSpline() const;

protected:

	UPROPERTY()
		USplineComponent* Spline;

};
