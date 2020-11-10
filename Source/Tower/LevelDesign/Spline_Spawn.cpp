// Fill out your copyright notice in the Description page of Project Settings.
#include "Spline_Spawn.h"
#include "Components/SplineComponent.h"

void ASpline_Spawn::Spawn(TSubclassOf<AActor> Class, float Alpha)
{
	UWorld* World = GetWorld();
	if (World == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("No world, can't spawn on spline"));
		return;
	}

	if (Spline == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("No spline, can't spawn on spline"));
		return;
	}

	float TotalSplineLength = Spline->GetSplineLength();
	float TartgetSplineDistance = TotalSplineLength * FMath::Clamp(Alpha, 0.0f, 1.0f);

	FVector Location = Spline->GetWorldLocationAtDistanceAlongSpline(TartgetSplineDistance);
	FVector Tangent = Spline->GetWorldTangentAtDistanceAlongSpline(TartgetSplineDistance);
	FRotator Rotation = Tangent.Rotation();

	if (bRaycastToGround)
	{
		FVector TopPos = Location + (FVector::UpVector * RayCastUpDist);
		FHitResult Hit;
		if (World->LineTraceSingleByChannel(Hit, Location, TopPos, RayCastCollectionChannel))
		{
			TopPos = Location + (FVector::UpVector * (Hit.Distance * 0.99f));
		}
		Location = TopPos;
	}

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	AActor* SpawnedActor = World->SpawnActor<AActor>(Class, Location, Rotation, SpawnParams);
	if (SpawnedActor == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to spawn"));
		return;
	}

	if (bRaycastToGround)
	{
		FVector CurrentPosition = SpawnedActor->GetActorLocation();
		FVector RayCastEnd = CurrentPosition + (FVector::DownVector * RayCastDownDist);
		FHitResult Hit;
		if (World->LineTraceSingleByChannel(Hit, CurrentPosition, RayCastEnd, RayCastCollectionChannel))
		{
			FVector BoundsOrigin;
			FVector BoundsExtent;
			SpawnedActor->GetActorBounds(false, BoundsOrigin, BoundsExtent);

			float MoveDistUp = (CurrentPosition.Z - BoundsOrigin.Z) + BoundsExtent.Z;

			FVector TargetPos = Hit.Location + (FVector::UpVector * MoveDistUp);


			SpawnedActor->SetActorLocation(TargetPos, false, nullptr, ETeleportType::ResetPhysics);
		}
	}
}
