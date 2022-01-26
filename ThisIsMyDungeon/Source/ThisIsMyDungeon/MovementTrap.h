// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Trap.h"
#include "MovementTrap.generated.h"

class UBoxComponent;

/**
 *
 */
UCLASS()
class THISISMYDUNGEON_API AMovementTrap : public ATrap
{
	GENERATED_BODY()


public:
	virtual void AddBuff() override { speed_ *= buffValue; };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Settings)
		int damage = 1.f;

	AMovementTrap();
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Settings)
		float speed_ = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Settings)
		float near = 10.f;

	// Mesh to see the entire trap location
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Settings)
		UStaticMeshComponent* meshDamage;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Settings)
		class UBoxComponent* hitBoxDamage;

	UFUNCTION()
		virtual void BeginOverlap(UPrimitiveComponent* OverlappedComponent,
			AActor* OtherActor,
			UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex,
			bool bFromSweep,
			const FHitResult& SweepResult) {};

	//virtual void Tick(float DeltaTime) {} override;

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Settings)
		float buffValue = 2.f;

	//virtual void BeginPlay() {} override;
};
