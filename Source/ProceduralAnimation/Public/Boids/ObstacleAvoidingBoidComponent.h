// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BoidMovementComponent.h"
#include "ObstacleAvoidingBoidComponent.generated.h"

/**
 * 
 */
UCLASS()
class PROCEDURALANIMATION_API UObstacleAvoidingBoidComponent : public UBoidMovementComponent
{
	GENERATED_BODY()

public:
	// Called every frame
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;

	// Range at which the boid will see an obstacle in front of it and act to avoid it.
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "*Boid|Properties|ObstacleAvoidance" )
	float ObstacleAvoidanceRange = 100.f;

	// Force at which the boid will act to avoid obstacles.
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "*Boid|Properties|ObstacleAvoidance" )
	float ObstacleAvoidanceForceMultiplier = 10.f;

	// Weight 0-1 for obstacle avoidance
	UPROPERTY( EditAnywhere, Category = "*Boid|Weights", meta = (ClampMin = "0.0"
		, ClampMax = "1.0", UIMin = "0.0", UIMax = "1.0") )
	float ObstacleAvoidanceWeight = 1;

	/** Returns a direction in which the boid needs to steer in order to avoid collision with an obstacle(not another boid) */
	UFUNCTION( BlueprintNativeEvent, BlueprintCallable, Category = "*Boid|CollisionDetection" )
	FVector GetUnobstructedDir( FHitResult Hit );

	/** Returns true if an obstacle is in the boid's current direction of movement. Updates the given hit. */
	UFUNCTION( BlueprintNativeEvent, BlueprintCallable, Category = "*Boid|CollisionDetection" )
	bool IsObstacleAhead( FHitResult& Hit );
	
};
