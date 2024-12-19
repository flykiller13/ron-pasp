// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/StaticMeshActor.h"
#include "SnakeSection.generated.h"

/**
 * 
 */
UCLASS()
class PROCEDURALANIMATION_API ASnakeSection : public AStaticMeshActor
{
	GENERATED_BODY()

public:
	/** Please add a function description */
	UFUNCTION( BlueprintCallable, Category = "Default" )
	void ConstrainDistance( ASnakeSection* ToSection );


	/** Please add a function description */
	UFUNCTION( BlueprintCallable, Category = "Default" )
	void ConstrainAngle();

public:

	/** Please add a variable description */
	UPROPERTY( BlueprintReadWrite, EditAnywhere, Category = "Default", meta = (ExposeOnSpawn = "true") )
	int32 SectionIndex;

	/** Please add a variable description */
	UPROPERTY( BlueprintReadWrite, EditAnywhere, Category = "Default", meta = (ExposeOnSpawn = "true") )
	int32 TotalSize;

	/** Please add a variable description */
	UPROPERTY( BlueprintReadWrite, EditInstanceOnly, Category = "Default" )
	TObjectPtr<ASnakeSection> PrevSection;

	/** Please add a variable description */
	UPROPERTY( BlueprintReadWrite, EditInstanceOnly, Category = "Default" )
	TObjectPtr<ASnakeSection> NextSection;

	/** Please add a variable description */
	UPROPERTY( BlueprintReadWrite, EditAnywhere, Category = "Default", meta = (ExposeOnSpawn = "true") )
	float DesiredDistance = 150.f;

	/** Please add a variable description */
	UPROPERTY( BlueprintReadWrite, EditAnywhere, Category = "Default", meta = (ExposeOnSpawn = "true") )
	float DesiredAngle = 120.f;

	
};
