// Fill out your copyright notice in the Description page of Project Settings.
#include "Spline_Spawn.h"
#include "Components/SplineComponent.h"
#include "../ActorComponents/SplineFollower.h"

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

	FVector Location;
	FRotator Rotation;
	USplineFollower::GetLocationAndRotationForAlpha(Spline, Alpha, Location, Rotation);
	float OriginalSplineZ = Location.Z;

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

	float ZOffset = SpawnedActor->GetActorLocation().Z - OriginalSplineZ;

	TArray<USplineFollower*> SplineFollowers;
	SpawnedActor->GetComponents<USplineFollower>(SplineFollowers);
	for (auto SplineFollower : SplineFollowers)
	{
		SplineFollower->Init(this, Alpha, ZOffset);
	}
}

void ASpline_Spawn::SpawnMany(TSubclassOf<AActor> Class, float Alpha, int32 NumToSpawn, float TimeBetweenSpawns)
{
	for (int i = 0; i < NumToSpawn; ++i)
	{
		float SpawnTime = TimeBetweenSpawns * (float)i;
		if (SpawnTime > 0.0f)
		{
			FTimerHandle Handle;
			FTimerDelegate Del;
			Del.BindUFunction(this, FName("Spawn"), Class, Alpha);
			GetWorldTimerManager().SetTimer(Handle, Del, SpawnTime, false);
		}
		else
		{
			Spawn(Class, Alpha);
		}
	}
}
