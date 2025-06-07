// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BoidMovementComponent.h"
#include "FlockManager.generated.h"

USTRUCT(Blueprintable, BlueprintType)
struct FlockData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	TArray<UBoidMovementComponent*> Boids;
};

/**
 * 
 */
UCLASS(Blueprintable)
class PROCEDURALANIMATION_API UFlockManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	// Draws the center of the perceived flock
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	FVector GetFlockCenter(int32 FlockID);

	// Draws the perceived flock's velocity
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	FVector GetFlockVelocity(int32 FlockID);

	UPROPERTY(BlueprintReadWrite)
	TMap<int32, FlockData> FlockMap;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void AddFlock(int32 FlockID);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void AddBoid( int32 FlockID, UBoidMovementComponent* Boid);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	TArray<UBoidMovementComponent*> GetNearbyBoids(int32 FlockID, UBoidMovementComponent* RequestingBoid, float Radius);
};
