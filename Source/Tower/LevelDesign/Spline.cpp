// Fill out your copyright notice in the Description page of Project Settings.
#include "Spline.h"
#include "Components/SplineComponent.h"

ASpline::ASpline()
{
	PrimaryActorTick.bCanEverTick = false;

	Spline = CreateDefaultSubobject<USplineComponent>(TEXT("Spline"));
}
