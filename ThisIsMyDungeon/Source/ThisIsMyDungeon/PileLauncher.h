// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ShooterTrap.h"
#include "PileLauncher.generated.h"

class AArrow;

/**
 * 
 */
UCLASS()
class THISISMYDUNGEON_API APileLauncher : public AShooterTrap
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;

	int targetNumber = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Settings, meta = (AllowPrivateAccess = "true"))
		float tickRate = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Settings, meta = (AllowPrivateAccess = "true"))
		float spawnDistance = 30.f;

	float timer = 0.f;

	void Shoot();

	UPROPERTY(EditDefaultsOnly, Category = Projectile)
		TSubclassOf<class AArrow> myArrow;

	class AArrow* newArrow = nullptr;

public:
	APileLauncher();
	virtual void Tick(float DeltaTime) override;

	//UFUNCTION()
		virtual void BeginOverlap(UPrimitiveComponent* OverlappedComponent,
			AActor* OtherActor,
			UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex,
			bool bFromSweep,
			const FHitResult& SweepResult) override;

	//UFUNCTION()
		virtual void EndOverlap(UPrimitiveComponent* OverlappedComponent,
			AActor* OtherActor,
			UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex) override;
};
