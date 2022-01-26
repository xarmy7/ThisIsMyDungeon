// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MovementTrap.h"
#include "Anvil.generated.h"

class UBoxComponent;

/**
 *
 */
UCLASS()
class THISISMYDUNGEON_API AAnvil : public AMovementTrap
{
	GENERATED_BODY()

public:
	AAnvil();

	virtual void BeginOverlap(UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult) override;

	UFUNCTION()
		void BeginOverlapDetection(UPrimitiveComponent* OverlappedComponent,
			AActor* OtherActor,
			UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex,
			bool bFromSweep,
			const FHitResult& SweepResult);

	UFUNCTION()
		void EndOverlapDetection(UPrimitiveComponent* OverlappedComponent,
			AActor* OtherActor,
			UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex);

	FVector Start;
	FVector End;

protected:
	virtual void BeginPlay() override;
	void TraceLine();

	int targetNumber = 0;

	float speed = 0.f;
	float maxStart = 240.f;
	float distance = 0.f;
	float refDistance = 0.f;
	float divided = 6.f;

	bool setValue = false;
	bool wasPlaced = false;
	bool goUp = false;
	bool activate = false;

	FVector direction = {};
	FVector hitLoc = {};
	FVector myVector = {};
	FVector refStart = {};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Settings)
		class UBoxComponent* detectionBox;

public:
	virtual void Tick(float DeltaTime) override;
};
