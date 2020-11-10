// Fill out your copyright notice in the Description page of Project Settings.
#pragma once
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SplineFollower.generated.h"

class USplineComponent;
class ASpline;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TOWER_API USplineFollower : public UActorComponent
{
	GENERATED_BODY()

public:	

	USplineFollower();

	UPROPERTY(EditAnywhere)
		float Speed = 100.0f;

protected:

	virtual void BeginPlay() override;

public:	

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	static void GetLocationAndRotationForAlpha(USplineComponent* Spline, float Alpha, FVector& Location, FRotator& Rotation);

	static void GetLocationAndRotationForDistance(USplineComponent* Spline, float Distance, FVector& Location, FRotator& Rotation);

	static float GetDistance(USplineComponent* Spline, float Alpha);

	UFUNCTION(BlueprintCallable)
		void Init(ASpline* SplineIn, float AlphaIn, float ZOffsetIn);

protected:

	UPROPERTY()
		ASpline* Spline;
	
	UPROPERTY()
		float InitialAlpha;

	UPROPERTY()
		float ZOffset;

	UPROPERTY()
		float CurrentDistance;

};
