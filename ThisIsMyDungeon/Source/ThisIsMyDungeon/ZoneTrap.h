// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Trap.h"
#include "ZoneTrap.generated.h"

/**
 * 
 */
UCLASS()
class THISISMYDUNGEON_API AZoneTrap : public ATrap
{
	GENERATED_BODY()
	
public:
	AZoneTrap();

	UFUNCTION()
		virtual void BeginOverlap(UPrimitiveComponent* OverlappedComponent,
			AActor* OtherActor,
			UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex,
			bool bFromSweep,
			const FHitResult& SweepResult) {};

	UFUNCTION()
		virtual void EndOverlap(UPrimitiveComponent* OverlappedComponent,
			AActor* OtherActor,
			UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex) {};

protected:
	virtual void BeginPlay() override;
};
