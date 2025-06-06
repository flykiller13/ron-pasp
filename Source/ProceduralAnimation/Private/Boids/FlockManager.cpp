// Fill out your copyright notice in the Description page of Project Settings.


#include "Boids/FlockManager.h"

FVector UFlockManager::GetFlockCenter_Implementation(int32 FlockID, bool bDrawDebug)
{
	FVector Center = FVector::ZeroVector;

	FlockData* Flock = FlockMap.Find(FlockID);

	for (UBoidMovementComponent* Boid : Flock->Boids)
	{
		Center += Boid->Owner->GetActorLocation();
	}

	Center = Center / Flock->Boids.Num(); // Get average location

	if (bDrawDebug)
	{
		DrawDebugSphere(GetWorld(), Center, 10.f, 2, FColor::Cyan);
	}
	return Center;
}

FVector UFlockManager::GetFlockVelocity_Implementation(int32 FlockID, bool bDrawDebug)
{
	FVector Velocity = FVector::ZeroVector;

	FlockData* Flock = FlockMap.Find(FlockID);

	for (UBoidMovementComponent* Boid : Flock->Boids)
	{
		Velocity += Boid->GetCurrentVelocity();
	}

	Velocity = Velocity / Flock->Boids.Num(); // Get average velocity

	if (bDrawDebug)
	{
		DrawDebugDirectionalArrow(GetWorld(), GetFlockCenter(FlockID), GetFlockCenter(FlockID) + Velocity, 1.f,
		                          FColor::Yellow);
	}

	return Velocity;
}

void UFlockManager::AddFlock_Implementation(int32 FlockID)
{
	FlockData Flock = FlockData{};

	FlockMap.Add(FlockID, Flock);
}

void UFlockManager::AddBoid_Implementation(int32 FlockID, UBoidMovementComponent* Boid)
{
	FlockData* Flock = FlockMap.Find(FlockID);

	Flock->Boids.Add(Boid);
}

TArray<UBoidMovementComponent*> UFlockManager::GetNearbyBoids_Implementation(int32 FlockID,
	UBoidMovementComponent* RequestingBoid, float Radius)
{
	FlockData* Flock = FlockMap.Find(FlockID);
	TArray<UBoidMovementComponent*> NearbyBoids;

	for (UBoidMovementComponent* Boid : Flock->Boids)
	{
		if (Boid != RequestingBoid)
		{
			NearbyBoids.Add(Boid);
		}
	}

	return NearbyBoids;
}
