// Fill out your copyright notice in the Description page of Project Settings.


#include "SnakeSection.h"

void ASnakeSection::ConstrainDistance(ASnakeSection* ToSection)
{
	if (!ToSection)
	{
		return;
	}

	FVector Dist = ToSection->GetActorLocation() - GetActorLocation();

	if (Dist.Length() > DesiredDistance)
	{
		FVector NewLocation = GetActorLocation() + (Dist / Dist.Length()) * DesiredDistance;
		ToSection->SetActorLocation( NewLocation );
	}
}

void ASnakeSection::ConstrainAngle()
{
	if (!NextSection || !PrevSection )
	{
		return;
	}

	FVector Prev = PrevSection->GetActorLocation() - GetActorLocation();
	FVector Next = NextSection->GetActorLocation() - GetActorLocation();

	// Get the angle that is created between prev -> this -> next
	float CurrAngle = FMath::Acos(Prev.GetSafeNormal().Dot(Next.GetSafeNormal()));

	if (CurrAngle < DesiredAngle)
	{
		// Clamp the angle created between the sections. The pole vector is the cross product of prev and next.
		FVector NewLocation = Prev.RotateAngleAxis( DesiredAngle, Prev.Cross( Next ) );
		NextSection->SetActorLocation( GetActorLocation() + NewLocation );
	}

}
