// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MovementTrap.h"
#include "Saw.generated.h"

/**
 * 
 */
UCLASS()
class THISISMYDUNGEON_API ASaw : public AMovementTrap
{
	GENERATED_BODY()

public:
	ASaw();

	FVector Start;
	FVector End;

	/*UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Settings)
		int damage = 1.f;*/

	/*UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Settings)
		float speed_ = 1.0f;*/

	/*UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Settings)
		float near = 10.f;*/

	// Mesh to see the entire trap location
	/*UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Settings)
		UStaticMeshComponent* meshSaw;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Settings)
		class UBoxComponent* hitBoxSaw;*/

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* bladeSupport;

	//UFUNCTION()
		virtual void BeginOverlap(UPrimitiveComponent* OverlappedComponent,
			AActor* OtherActor,
			UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex,
			bool bFromSweep,
			const FHitResult& SweepResult) override;

protected:
	virtual void BeginPlay() override;

	FVector TargetPoint;

	FVector direction;

	bool setValue = false;

	void SetValue();

	FVector Offset = FVector(0.f, 0.f, 0.f);

public:
	virtual void Tick(float DeltaTime) override;
};
