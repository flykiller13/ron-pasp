// Fill out your copyright notice in the Description page of Project Settings.


#include "Boids/ObstacleAvoidingBoidComponent.h"

void UObstacleAvoidingBoidComponent::TickComponent( float DeltaTime, ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction )
{
	// Steer away from obstacles
	FHitResult Hit;
	if ( IsObstacleAhead( Hit ) )
	{
		CurrentVelocity += GetUnobstructedDir( Hit ) * ObstacleAvoidanceWeight;
	}

	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );
}

FVector UObstacleAvoidingBoidComponent::GetUnobstructedDir_Implementation(FHitResult Hit)
{
	FVector Offset = Hit.ImpactPoint - GetActorLocation();
	float Dot = GetCurrentVelocity().Dot( Offset );
	FVector UnobstructedDir( 0 );

	if ( Dot <= 0 )
	{
		UnobstructedDir = GetCurrentVelocity() + Hit.Normal * ObstacleAvoidanceForceMultiplier;
	}
	else
	{
		UnobstructedDir = Hit.Normal * ObstacleAvoidanceForceMultiplier;
	}

	return UnobstructedDir;
}

bool UObstacleAvoidingBoidComponent::IsObstacleAhead_Implementation(FHitResult& Hit)
{
	// Trace settings
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor( this );
	// CollisionParams.bTraceComplex = true;

	FVector TraceStart = GetActorLocation();
	FVector TraceEnd = GetActorLocation() + CurrentVelocity.GetSafeNormal() * ObstacleAvoidanceRange;

	FVector MeshBounds = BoidMesh->GetStaticMesh()->GetBounds().GetBox().GetSize();
	float SphereRadius = (BoidMesh->GetComponentScale().X * MeshBounds.X) / 2;
	FCollisionShape MySphere = FCollisionShape::MakeSphere( SphereRadius );

	return GetWorld()->SweepSingleByChannel( Hit, TraceStart, TraceEnd, FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel1, MySphere, CollisionParams );
}
