// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "FlockManager.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class PROCEDURALANIMATION_API UFlockManager : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	// Draws the center of the perceived flock
	UFUNCTION( BlueprintNativeEvent, BlueprintCallable, Category = "Debugging" )
	FVector GetFlockCenter( int32 FlockID, bool bDrawDebug = false );

	// Draws the perceived flock's velocity
	UFUNCTION( BlueprintNativeEvent, BlueprintCallable, Category = "Debugging" )
	FVector GetFlockVelocity( int32 FlockID, bool bDrawDebug = false );

};
