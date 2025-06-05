// Fill out your copyright notice in the Description page of Project Settings.


#include "BoidMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "DrawDebugHelpers.h"
#include "BoidManager.h"
#include "Engine/World.h"


UBoidMovementComponent::UBoidMovementComponent()
{
}

// Called when the game starts
void UBoidMovementComponent::BeginPlay()
{
	Super::BeginPlay();

	AActor* Owner = GetOwner();

	if ( bOverrideStart )
	{
		// Update velocity
		CurrentVelocity = StartDir * StartSpeed;
	}
	else
	{
		// Give the boid a random direction and speed
		CurrentVelocity = FMath::VRand() * ((MaxSpeed + MinSpeed) / 2);
	}

	Owner->GetWorldTimerManager().SetTimer( BoidTimerHandle, this, &UBoidMovementComponent::BoidTick, BoidTickTime, true );
	
}


// Called every frame
void UBoidMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FVector FlockVel( 0 );
	FVector FlockCenter( 0 );
	FVector SeparationForce( 0 );

	if ( BoidManager )
	{

		FlockVel = GetFlockVelocity() * AlignmentWeight;
		DrawFlockVelocity( FlockVel );

		FlockCenter = GetOffsetToFlockCenter() * CohesionWeight;

		SeparationForce = GetSeparationForce() * SeparationWeight;
		DrawSeparationForce( SeparationForce );

	}

	CurrentVelocity += FlockVel + FlockCenter + SeparationForce; // Add rules to velocity
	CurrentVelocity += (GetAttractionPointForce() * AttractionPointWeight); // Add force to move towards the attraction point (if exists)

	CurrentVelocity = CurrentVelocity.GetClampedToSize( MinSpeed, MaxSpeed ); // Clamp velocity to min-max speed
	DrawBoidVelocity();

	float InterpSpeed = 10;

	//Moves the Boid
	FVector TargetLocation = Owner->GetActorLocation() + CurrentVelocity;
	FVector NewLocation = FMath::VInterpTo( Owner->GetActorLocation(), TargetLocation, BoidTickTime, InterpSpeed );
	Owner->SetActorLocation( NewLocation );

	////Makes Rot from Z cause Boid mesh points to Z direction
	//FRotator TargetRotation = UKismetMathLibrary::MakeRotFromZ( CurrentVelocity );
	//FRotator NewRotation = FMath::RInterpTo( BoidMesh->GetRelativeRotation(), TargetRotation, BoidTickTime, InterpSpeed );
	//BoidMesh->SetRelativeRotation( NewRotation );
}

// Return Current Velocity.
FVector UBoidMovementComponent::GetCurrentVelocity()
{
	return CurrentVelocity;
}

void UBoidMovementComponent::BoidTick()
{
	
}

FVector UBoidMovementComponent::GetOffsetToFlockCenter_Implementation()
{
	FVector Center( 0 );
	int32 FlockCounter = 0;

	if ( !BoidManager->Boids.IsEmpty() )
	{
		for ( auto& TempBoid : BoidManager->Boids )
		{
			FVector Offset = Owner->GetActorLocation() - TempBoid->GetActorLocation();
			if ( TempBoid != this && Offset.Length() <= VisionRange )
			{
				Center += TempBoid->GetActorLocation();
				FlockCounter++;
			}
		}
		if ( FlockCounter > 0 )
		{
			Center /= FlockCounter;
			DrawFlockCenter( Center );
			return (Center - Owner->GetActorLocation()) / 100;
		}

		return Owner->GetActorLocation(); // Only boid in the flock, center of flock is boid itself
	}

	return Center;
}

FVector UBoidMovementComponent::GetFlockVelocity_Implementation()
{
	FVector AvgVelocity( 0 );
	int32 FlockCounter = 0;

	if ( !BoidManager->Boids.IsEmpty() )
	{
		for ( auto& TempBoid : BoidManager->Boids )
		{
			FVector Offset = Owner->GetActorLocation() - TempBoid->GetActorLocation();
			if ( TempBoid != this && Offset.Length() <= VisionRange )
			{
				AvgVelocity += TempBoid->CurrentVelocity;
				FlockCounter++;
			}
		}
		if ( FlockCounter > 0 )
			AvgVelocity /= FlockCounter;
	}

	return (AvgVelocity - GetCurrentVelocity()) / 20;
}

// Returns a vector that encourages the Boid to steer AWAY from nearby Boids.
FVector UBoidMovementComponent::GetSeparationForce_Implementation()
{
	FVector SeparationDir( 0 );

	for ( auto& Boid : BoidManager->Boids )
	{
		if ( Boid != this )
		{
			FVector Offset = Boid->GetActorLocation() - this->GetActorLocation();

			if ( Offset.Length() > 0 && Offset.SquaredLength() <= SeparationRange * SeparationRange )
			{
				Offset = Offset / Offset.Length();
				SeparationDir -= (Offset);
			}
		}
	}

	return SeparationDir;
}

FVector UBoidMovementComponent::GetAttractionPointForce_Implementation()
{
	FVector Offset( 0 );

	if ( AttractionPoint )
	{
		Offset = AttractionPoint->Owner->GetActorLocation() - this->Owner->GetActorLocation();
	}

	return Offset / 100;
}

void UBoidMovementComponent::DrawBoidVelocity_Implementation()
{
	if ( bDrawBoidVelocity )
		DrawDebugDirectionalArrow( GetWorld(), Owner->GetActorLocation(), Owner->GetActorLocation() + GetVelocity(), 1.f, FColor::Blue, false,
			BoidTickTime, 0U, 1.f );
}

void UBoidMovementComponent::DrawFlockCenter_Implementation( FVector Center )
{
	// Put this in getoffsettoflockcenter()?
	if ( bDrawFlockCenter )
		DrawDebugSphere( GetWorld(), Center, 10.f, 2, FColor::Cyan );
}

void UBoidMovementComponent::DrawSeparationForce_Implementation( FVector Force )
{
	if ( bDrawSeparationForce )
		DrawDebugDirectionalArrow( GetWorld(), Owner->GetActorLocation(), Owner->GetActorLocation() + Force, 1.f, FColor::Purple );
}

void UBoidMovementComponent::DrawFlockVelocity_Implementation( FVector FlockVelocity )
{
	if ( bDrawFlockVelocity )
		DrawDebugDirectionalArrow( GetWorld(), Owner->GetActorLocation(), Owner->GetActorLocation() + FlockVelocity, 1.f, FColor::Yellow );
}


