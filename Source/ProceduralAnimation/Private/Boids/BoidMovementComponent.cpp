// Fill out your copyright notice in the Description page of Project Settings.


#include "BoidMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"


UBoidMovementComponent::UBoidMovementComponent()
{
}

// Called when the game starts
void UBoidMovementComponent::BeginPlay()
{
	Super::BeginPlay();

	Owner = GetOwner();

	if (bOverrideStart)
	{
		// Update velocity
		CurrentVelocity = StartDir * StartSpeed;
	}
	else
	{
		// Give the boid a random direction and speed
		CurrentVelocity = FMath::VRand() * ((MaxSpeed + MinSpeed) / 2);
	}

}


// Called every frame
void UBoidMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                           FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FVector FlockVel(0);
	FVector FlockCenter(0);
	FVector SeparationForce(0);

	if (FlockManager)
	{
		FlockVel = GetOffsetToFlockVelocity() * AlignmentWeight;

		FlockCenter = GetOffsetToFlockCenter() * CohesionWeight;

		SeparationForce = GetSeparationForce() * SeparationWeight;
		DrawSeparationForce(SeparationForce);
	}

	CurrentVelocity += FlockVel + FlockCenter + SeparationForce; // Add rules to velocity
	CurrentVelocity += (GetAttractionPointForce() * AttractionPointWeight);
	// Add force to move towards the attraction point (if exists)

	CurrentVelocity = CurrentVelocity.GetClampedToSize(MinSpeed, MaxSpeed); // Clamp velocity to min-max speed

	float InterpSpeed = 10;

	//Moves the Boid
	FVector TargetLocation = Owner->GetActorLocation() + CurrentVelocity;
	FVector NewLocation = FMath::VInterpTo(Owner->GetActorLocation(), TargetLocation, DeltaTime, InterpSpeed);
	Owner->SetActorLocation(NewLocation);

	////Makes Rot from Z cause Boid mesh points to Z direction
	//FRotator TargetRotation = UKismetMathLibrary::MakeRotFromZ( CurrentVelocity );
	//FRotator NewRotation = FMath::RInterpTo( BoidMesh->GetRelativeRotation(), TargetRotation, BoidTickTime, InterpSpeed );
	//BoidMesh->SetRelativeRotation( NewRotation );
}

// Return Current Velocity.
FVector UBoidMovementComponent::GetCurrentVelocity()
{
	DrawDebugDirectionalArrow(GetWorld(), Owner->GetActorLocation(), Owner->GetActorLocation() + CurrentVelocity, 1.f,
								  FColor::Blue, false,
								  GetWorld()->GetDeltaSeconds(), 0U, 1.f);
	
	return CurrentVelocity;
}

FVector UBoidMovementComponent::GetOffsetToFlockCenter_Implementation()
{
	FVector FlockCenter = FlockManager->GetFlockCenter(FlockID);

	return FlockCenter - Owner->GetActorLocation();
}

FVector UBoidMovementComponent::GetOffsetToFlockVelocity_Implementation()
{
	FVector
		FlockVelocity = FlockManager->GetFlockVelocity(FlockID);

	return FlockVelocity - GetCurrentVelocity();
}

// Returns a vector that encourages the Boid to steer AWAY from nearby Boids.
FVector UBoidMovementComponent::GetSeparationForce_Implementation()
{
	FVector SeparationDir(0);
	
	// for (auto& Boid : FlockManager->Boids)
	// {
	// 	if (Boid != this)
	// 	{
	// 		FVector Offset = Boid->GetActorLocation() - this->GetActorLocation();
	//
	// 		if (Offset.Length() > 0 && Offset.SquaredLength() <= SeparationRange * SeparationRange)
	// 		{
	// 			Offset = Offset / Offset.Length();
	// 			SeparationDir -= (Offset);
	// 		}
	// 	}
	// }

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

void UBoidMovementComponent::DrawSeparationForce_Implementation(FVector Force)
{
	if (bDrawSeparationForce)
		DrawDebugDirectionalArrow(GetWorld(), Owner->GetActorLocation(), Owner->GetActorLocation() + Force, 1.f, FColor::Purple);
}
