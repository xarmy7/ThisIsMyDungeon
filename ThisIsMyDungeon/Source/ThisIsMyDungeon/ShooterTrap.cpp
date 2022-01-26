// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterTrap.h"
#include "DrawDebugHelpers.h"

void AShooterTrap::SetRangeDetectionBox()
{
	FVector startPoint = DectectionBox->GetComponentLocation();
	FVector forward = DectectionBox->GetUpVector();

	FVector targetLocation = startPoint + forward * maxRange;

	FHitResult hit;
	
	if (GetWorld()->LineTraceSingleByChannel(hit, startPoint, targetLocation,ECollisionChannel::ECC_GameTraceChannel8))
	{
		range = FVector::Dist(hit.Location, startPoint);
	}
	else
	{
		range = maxRange;
	}

	range = range * .5f;

	FVector scale = DectectionBox->GetScaledBoxExtent();
	scale.Z = 0.f;

	DectectionBox->SetBoxExtent(scale + FVector(0.f, 0.f, range));
	DectectionBox->SetRelativeLocation(FVector(0.f, 0.f, range));
}