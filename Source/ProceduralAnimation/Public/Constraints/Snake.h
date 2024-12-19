// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "SnakeSection.h"
#include "Snake.generated.h"

UCLASS()
class PROCEDURALANIMATION_API ASnake : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ASnake();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:

	/** Please add a function description */
	UFUNCTION( BlueprintImplementableEvent, BlueprintCallable )
	void CreateSections( int32 NumSections );

	/** Please add a variable description */
	UPROPERTY( BlueprintReadWrite, EditDefaultsOnly, Category = "Default" )
	TObjectPtr<ASnakeSection> Head;

	/** Please add a variable description */
	UPROPERTY( BlueprintReadWrite, EditAnywhere, Category = "Default" )
	int32 Sections;

	/** Please add a variable description */
	UPROPERTY( BlueprintReadWrite, EditAnywhere, Category = "Default" )
	float DistanceConstraint;

	/** Please add a variable description */
	UPROPERTY( BlueprintReadWrite, EditAnywhere, Category = "Default" )
	float AngleConstraint;


};
