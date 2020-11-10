// Fill out your copyright notice in the Description page of Project Settings.
#include "SplineFollower.h"
#include "../LevelDesign/Spline.h"
#include "Components/SplineComponent.h"

USplineFollower::USplineFollower()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void USplineFollower::BeginPlay()
{
	Super::BeginPlay();
}

void USplineFollower::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (Spline == nullptr)
	{
		return;
	}

	float TotalSplineLength = Spline->GetSpline()->GetSplineLength();
	CurrentDistance = FMath::Min(TotalSplineLength, CurrentDistance + (Speed * DeltaTime));

	FVector Location;
	FRotator Rotation;
	GetLocationAndRotationForDistance(Spline->GetSpline(), CurrentDistance, Location, Rotation);

	GetOwner()->SetActorLocationAndRotation(Location + (FVector::UpVector * ZOffset), Rotation);
}

void USplineFollower::GetLocationAndRotationForAlpha(USplineComponent* Spline, float Alpha, FVector& Location, FRotator& Rotation)
{
	float TartgetSplineDistance = GetDistance(Spline, Alpha);
	GetLocationAndRotationForDistance(Spline, TartgetSplineDistance, Location, Rotation);
}

void USplineFollower::GetLocationAndRotationForDistance(USplineComponent* Spline, float Distance, FVector& Location, FRotator& Rotation)
{
	Location = Spline->GetWorldLocationAtDistanceAlongSpline(Distance);
	FVector Tangent = Spline->GetWorldTangentAtDistanceAlongSpline(Distance);
	Rotation = Tangent.Rotation();
}

float USplineFollower::GetDistance(USplineComponent* Spline, float Alpha)
{
	float TotalSplineLength = Spline->GetSplineLength();
	return TotalSplineLength * FMath::Clamp(Alpha, 0.0f, 1.0f);
}

void USplineFollower::Init(ASpline* SplineIn, float AlphaIn, float ZOffsetIn)
{
	Spline = SplineIn;
	InitialAlpha = AlphaIn;
	ZOffset = ZOffsetIn;
	CurrentDistance = GetDistance(Spline->GetSpline(), InitialAlpha);
}
