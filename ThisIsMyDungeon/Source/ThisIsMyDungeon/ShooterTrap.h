// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Trap.h"
#include "ShooterTrap.generated.h"

/**
 *
 */
UCLASS()
class THISISMYDUNGEON_API AShooterTrap : public ATrap
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Settings)
		float attackTime = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Settings)
		float maxRange = 100.f;

	float range = 1000.f;

	virtual void Shoot() {};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Settings)
		UBoxComponent* DectectionBox;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Settings)
		float buffValue = 2;

	bool initialized = false;

public:
	void SetRangeDetectionBox();

	virtual void AddBuff() override { attackTime *= buffValue; };

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

};
