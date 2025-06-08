// Fill out your copyright notice in the Description page of Project Settings.


#include "BoidMovementComponent.h"
#include "DrawDebugHelpers.h"
#include "FlockManager.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"


UBoidMovementComponent::UBoidMovementComponent()
{
	// Set default values. These are values that I found to work well as a demonstration for the boids.
	MinSpeed = 1000.f;
	MaxSpeed = 2000.f;
	
	InterpSpeed = 5.f;
	
	SeparationRange = 500.f;
	SeparationWeight = 1500.f;
	
	AlignmentWeight = 100.f;
	
	CohesionWeight = 5000.f;
	
	AttractionPointWeight = 1.f;
	
	// Flock ID defaults to 0
	FlockID = 0;
}

// Called when the game starts
void UBoidMovementComponent::BeginPlay()
{
	Super::BeginPlay();
	UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(GetWorld());
	FlockManager = GameInstance->GetSubsystem<UFlockManager>();
	Owner = GetOwner();

	// Give the boid a random direction and speed
	CurrentVelocity = FMath::VRand() * ((MaxSpeed + MinSpeed) / 2);
}


// Called every frame
void UBoidMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                           FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!FlockManager || !Owner) return;
	
	AlignmentForce = GetOffsetToFlockVelocity().GetClampedToMaxSize(1.f) * AlignmentWeight;

	CohesionForce = GetOffsetToFlockCenter().GetClampedToMaxSize(1.f) * CohesionWeight;

	SeparationForce = GetSeparationForce().GetClampedToMaxSize(1.f) * SeparationWeight * FMath::Exp(5.f); // SeparationForce is multiplied by Exp(5) to make it stronger. By default it's very small.

	// Calculate the steering Force
	SteeringForce = AlignmentForce + CohesionForce + SeparationForce; 

	// Add force to move towards the attraction point (if exists)
	// SteeringForce += (GetAttractionPointForce() * AttractionPointWeight);
	
	// Smooth acceleration (interpolate current velocity towards steering force)
	CurrentVelocity = FMath::VInterpTo(CurrentVelocity, SteeringForce, DeltaTime, InterpSpeed);

	// Clamp to speed limits
	CurrentVelocity = CurrentVelocity.GetClampedToSize(MinSpeed, MaxSpeed);
	
	// Apply Movement
	Owner->AddActorWorldOffset(CurrentVelocity*DeltaTime);
}

// Return Current Velocity.
FVector UBoidMovementComponent::GetCurrentVelocity()
{
	return CurrentVelocity;
}

FVector UBoidMovementComponent::GetOffsetToFlockCenter_Implementation()
{
	if (!FlockManager || !Owner) return FVector::ZeroVector;
	
	FVector FlockCenter = FlockManager->GetFlockCenter(FlockID);

	return FlockCenter - Owner->GetActorLocation();
}

FVector UBoidMovementComponent::GetOffsetToFlockVelocity_Implementation()
{
	if (!FlockManager || !Owner) return FVector::ZeroVector;
	
	FVector FlockVelocity = FlockManager->GetFlockVelocity(FlockID);

	return FlockVelocity - GetCurrentVelocity();
}

// Returns a vector that encourages the Boid to steer AWAY from nearby Boids.
FVector UBoidMovementComponent::GetSeparationForce_Implementation()
{
	if (!FlockManager || !Owner) return FVector::ZeroVector;
	
	FVector SeparationDir = FVector::ZeroVector;
	TArray<UBoidMovementComponent*> NearbyBoids = FlockManager->GetNearbyBoids(FlockID, this, SeparationRange);
	
	for (auto& Boid : NearbyBoids)
	{
		if (!IsValid(Boid) || !IsValid(Boid->GetOwner())) continue;
		
		FVector ToBoid = Boid->Owner->GetActorLocation() - Owner->GetActorLocation();
		float Distance = ToBoid.Size();

		if (Distance > 0 && Distance <= SeparationRange)
		{
			// Force stronger for closer boids
			float Strength = 1.0f / Distance;
			
			SeparationDir -= ToBoid.GetSafeNormal() * Strength;
		}
	}

	return SeparationDir;
}

FVector UBoidMovementComponent::GetAttractionPointForce_Implementation()
{
	FVector Offset(0);

	if (AttractionPoint)
	{
		Offset = AttractionPoint->Owner->GetActorLocation() - this->Owner->GetActorLocation();
	}

	return Offset / 100;
}
