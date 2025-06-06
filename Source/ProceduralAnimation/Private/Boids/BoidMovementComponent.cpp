// Fill out your copyright notice in the Description page of Project Settings.


#include "BoidMovementComponent.h"
#include "DrawDebugHelpers.h"
#include "FlockManager.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"


UBoidMovementComponent::UBoidMovementComponent()
{
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

	FVector AlignmentForce = FVector::ZeroVector;
	FVector CohesionForce = FVector::ZeroVector;
	FVector SeparationForce = FVector::ZeroVector;

	if (FlockManager)
	{
		AlignmentForce = GetOffsetToFlockVelocity() * AlignmentWeight;

		CohesionForce = GetOffsetToFlockCenter() * CohesionWeight;

		SeparationForce = GetSeparationForce() * SeparationWeight;
	}

	CurrentVelocity += AlignmentForce + CohesionForce + SeparationForce; // Add rules to velocity
	// CurrentVelocity += (GetAttractionPointForce() * AttractionPointWeight);
	// Add force to move towards the attraction point (if exists)

	CurrentVelocity = CurrentVelocity.GetClampedToSize(MinSpeed, MaxSpeed); // Clamp velocity to min-max speed

	//Moves the Boid
	FVector TargetLocation = Owner->GetActorLocation() + CurrentVelocity;
	FVector NewLocation = FMath::VInterpTo(Owner->GetActorLocation(), TargetLocation, DeltaTime, InterpSpeed);

	// Consume Velocity
	Owner->SetActorLocation(NewLocation);
	CurrentVelocity = FVector::ZeroVector;
}

// Return Current Velocity.
FVector UBoidMovementComponent::GetCurrentVelocity()
{
	return CurrentVelocity;
}

FVector UBoidMovementComponent::GetOffsetToFlockCenter_Implementation()
{
	FVector FlockCenter = FlockManager->GetFlockCenter(FlockID, true);

	return FlockCenter - Owner->GetActorLocation();
}

FVector UBoidMovementComponent::GetOffsetToFlockVelocity_Implementation()
{
	FVector FlockVelocity = FlockManager->GetFlockVelocity(FlockID);

	return FlockVelocity - GetCurrentVelocity();
}

// Returns a vector that encourages the Boid to steer AWAY from nearby Boids.
FVector UBoidMovementComponent::GetSeparationForce_Implementation()
{
	FVector SeparationDir = FVector::ZeroVector;
	TArray<UBoidMovementComponent*> NearbyBoids = FlockManager->GetNearbyBoids(FlockID, this, VisionRange);
	
	for (auto& Boid : NearbyBoids)
	{
		FVector Offset = Boid->Owner->GetActorLocation() - this->Owner->GetActorLocation();

		if (Offset.Length() > 0 && Offset.SquaredLength() <= SeparationRange * SeparationRange)
		{
			Offset = Offset / Offset.Length();
			SeparationDir -= (Offset);
		}
	}

	// DrawDebugDirectionalArrow(GetWorld(), Owner->GetActorLocation(), Owner->GetActorLocation() + Force, 1.f, FColor::Purple);

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
