// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BoidMovementComponent.generated.h"

class UFlockManager;

UCLASS(Blueprintable)
class PROCEDURALANIMATION_API UBoidMovementComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UBoidMovementComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(BlueprintReadOnly)
	AActor* Owner;

	// The current velocity vector of the boid.
	UPROPERTY(BlueprintReadOnly)
	FVector CurrentVelocity = FVector::ZeroVector;

	// The current velocity vector of the boid.
	UPROPERTY(BlueprintReadOnly)
	FVector SteeringForce = FVector::ZeroVector;

	/* Properties */
	
	// Minimum speed of the boid.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "*Boid|Speed")
	float MinSpeed = 500.f;

	// Maximum speed of the boid.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "*Boid|Speed")
	float MaxSpeed = 10000.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "*Boid|Properties")
	float InterpSpeed = 5.f;

	// The boid manager that keeps reference of the boid.
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "*Boid|Properties", meta = (ExposeOnSpawn = "true"))
	UFlockManager* FlockManager;

	// The boid manager that keeps reference of the boid.
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "*Boid|Properties", meta = (ExposeOnSpawn = "true"))
	int32 FlockID;

	// Actor to which the boids will attract. A lone boid without an attraction point will fly straight forward.
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "*Boid|Properties", meta = (ExposeOnSpawn = "true"))
	AActor* AttractionPoint;

	/* Weights */

	// Weight 0-1 for Alignment rule
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "*Boid|Alignment")
	float AlignmentWeight = 1.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "*Boid|Alignment")
	FVector AlignmentForce = FVector::ZeroVector;

	// Weight 0-1 for Separation rule
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "*Boid|Separation")
	float SeparationWeight = 1.5f;

	// The range that the boid will try to keep away from it's perceived flockmates
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "*Boid|Separation")
	float SeparationRange = 100.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "*Boid|Separation")
	FVector SeparationForce = FVector::ZeroVector;

	// Weight 0-1 for Cohesion rule
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "*Boid|Cohesion")
	float CohesionWeight = 500.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "*Boid|Cohesion")
	FVector CohesionForce = FVector::ZeroVector;

	// Weight 0-1 for the attraction point
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "*Boid|Attraction")
	float AttractionPointWeight = 0.1;

	/* Functions */

	/** Returns the center of the boid's immediate flockmates */
	UFUNCTION(BlueprintNativeEvent, BlueprintPure, BlueprintCallable, Category = "*Boid|Rules")
	FVector GetOffsetToFlockCenter();

	/** Returns the center of the boid's immediate flockmates */
	UFUNCTION(BlueprintNativeEvent, BlueprintPure, BlueprintCallable, Category = "*Boid|Rules")
	FVector GetOffsetToFlockVelocity();

	/** Returns vector to steer away from the boid's immediate flockmates */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "*Boid|Rules")
	FVector GetSeparationForce();

	/** Returns vector in the direction of the attraction point */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "*Boid|AttractionPoint")
	FVector GetAttractionPointForce();

	/** Returns the boid's current velocity. */
	UFUNCTION(BlueprintPure, BlueprintCallable, Category = "*Boid|Movement")
	FVector GetCurrentVelocity();
};
