// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BoidMovementComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
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
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY( BlueprintReadOnly )
	AActor* Owner;

	// The current velocity vector of the boid.
	UPROPERTY( BlueprintReadOnly )
	FVector CurrentVelocity = FVector( 0 );

	/* Properties */

	// A float that represents the boid's vision range.
	UPROPERTY( EditAnywhere, BlueprintReadOnly, Category = "*Boid|Properties" )
	float VisionRange = 300.f;

	// The range that the boid will try to keep away from it's perscepted flockmates
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "*Boid|Properties" )
	float SeparationRange = 130.f;

	/*
	* Bool to override the random start for the boid.
	*/
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "*Boid|Properties" )
	bool bOverrideStart;

	// Direction in which the boid will start it's flight (if overriden)
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "*Boid|Properties", meta = (EditCondition = "bOverrideStart") )
	FVector StartDir;

	// Speed at which the boid will start it's flight (if overriden)
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "*Boid|Properties", meta = (EditCondition = "bOverrideStart") )
	float StartSpeed = 60.f;

	// Minimum speed of the boid.
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "*Boid|Properties" )
	float MinSpeed = 20.f;

	// Maximum speed of the boid.
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "*Boid|Properties" )
	float MaxSpeed = 120.f;

	// The boid manager that keeps reference of the boid.
	UPROPERTY( VisibleAnywhere, BlueprintReadWrite, Category = "*Boid|Properties", meta = (ExposeOnSpawn = "true") )
	UBoidManager* BoidManager;

	// Actor to which the boids will attract. A lone boid without an attraction point will fly straight forward.
	UPROPERTY( VisibleAnywhere, BlueprintReadWrite, Category = "*Boid|Properties", meta = (ExposeOnSpawn = "true") )
	AActor* AttractionPoint;

	/* Weights */

	// Weight 0-1 for Alignment rule
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "*Boid|Weights", meta = (ClampMin = "0.0"
		, ClampMax = "1.0", UIMin = "0.0", UIMax = "1.0") )
	float AlignmentWeight = 0.2;

	// Weight 0-1 for Separation rule
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "*Boid|Weights", meta = (ClampMin = "0.0"
		, ClampMax = "1.0", UIMin = "0.0", UIMax = "1.0") )
	float SeparationWeight = 0.7;

	// Weight 0-1 for Cohesion rule
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "*Boid|Weights", meta = (ClampMin = "0.0"
		, ClampMax = "1.0", UIMin = "0.0", UIMax = "1.0") )
	float CohesionWeight = 0.5;

	// Weight 0-1 for the attraction point
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "*Boid|Weights", meta = (ClampMin = "0.0"
		, ClampMax = "1.0", UIMin = "0.0", UIMax = "1.0") )
	float AttractionPointWeight = 0.1;

	/* Functions */

	/** Returns the average velocity of the boid's immediate flockmates */
	UFUNCTION( BlueprintNativeEvent, BlueprintPure, BlueprintCallable, Category = "*Boid|Rules" )
	FVector GetFlockVelocity();

	/** Returns the center of the boid's immediate flockmates */
	UFUNCTION( BlueprintNativeEvent, BlueprintPure, BlueprintCallable, Category = "*Boid|Rules" )
	FVector GetOffsetToFlockCenter();

	/** Returns vector to steer away from the boid's immediate flockmates */
	UFUNCTION( BlueprintNativeEvent, BlueprintCallable, Category = "*Boid|Rules" )
	FVector GetSeparationForce();

	/** Returns vector in the direction of the attraction point */
	UFUNCTION( BlueprintNativeEvent, BlueprintCallable, Category = "*Boid|AttractionPoint" )
	FVector GetAttractionPointForce();

	/** Returns the boid's current velocity. */
	UFUNCTION( BlueprintPure, BlueprintCallable, Category = "*Boid|Movement" )
	FVector GetCurrentVelocity();

	/** This is the Boid's tick function. The interval can be controlled with BoidTickTime */
	UFUNCTION( BlueprintCallable, Category = "*Boid|Movement" )
	void BoidTick();

	FTimerHandle BoidTimerHandle;

	float BoidTickTime = 0.01f;

	/* DEBUGGING */

	// Draws the velocity of the boid 
	UFUNCTION( BlueprintNativeEvent, BlueprintCallable, Category = "*Boid|Debugging" )
	void DrawBoidVelocity();

	// True - Draws the velocity of the boid 
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "*Boid|Debugging" )
	bool bDrawBoidVelocity;

	// Draws the center of the perceived flock
	UFUNCTION( BlueprintNativeEvent, BlueprintCallable, Category = "*Boid|Debugging" )
	void DrawFlockCenter( FVector Center );

	// True - Draws the center of the perceived flock.
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "*Boid|Debugging" )
	bool bDrawFlockCenter;

	// Draws the force applied to steer away from the perceived flock
	UFUNCTION( BlueprintNativeEvent, BlueprintCallable, Category = "*Boid|Debugging" )
	void DrawSeparationForce( FVector Force );

	// True - Draws the force applied to steer away from the perceived flock
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "*Boid|Debugging" )
	bool bDrawSeparationForce;

	// Draws the perceived flock's velocity
	UFUNCTION( BlueprintNativeEvent, BlueprintCallable, Category = "*Boid|Debugging" )
	void DrawFlockVelocity( FVector FlockVelocity );

	// True - Draws the perceived flock's velocity
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "*Boid|Debugging" )
	bool bDrawFlockVelocity;

};
