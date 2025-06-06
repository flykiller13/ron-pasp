// Fill out your copyright notice in the Description page of Project Settings.


#include "Boids/FlockManager.h"

FVector UFlockManager::GetFlockCenter_Implementation(int32 FlockID, bool bDrawDebug )
{
	FVector Center;

	if ( bDrawDebug )
		DrawDebugSphere( GetWorld(), Center, 10.f, 2, FColor::Cyan );
	return FVector();
}

FVector UFlockManager::GetFlockVelocity_Implementation(int32 FlockID, bool bDrawDebug )
{
	FVector Velocity;

	if ( bDrawDebug )
		DrawDebugDirectionalArrow( GetWorld(), GetFlockCenter(FlockID), GetFlockCenter(FlockID) + Velocity, 1.f, FColor::Yellow );
	return FVector();
}
