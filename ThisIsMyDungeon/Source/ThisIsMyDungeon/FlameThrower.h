// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ShooterTrap.h"
#include "FlameThrower.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnFlameThrower);

class ANonPlayableCharacter;

/**
 *
 */
UCLASS()
class THISISMYDUNGEON_API AFlameThrower : public AShooterTrap
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AFlameThrower();

	virtual void BeginOverlap(UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult) override;

	virtual void EndOverlap(UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


	int targetNumber = 0;

	bool activate = false;

	TArray<ANonPlayableCharacter*> enemies;

	virtual void Shoot() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Settings)
	float reloadTime = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Settings)
	float shootDuration = 0.f;

	float baseShootDuration;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Settings)
	float burningInterval;

	float currentBurningTime = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Settings)
	int damage = 0;

	UPROPERTY(BlueprintAssignable, Category = Settings)
	FOnFlameThrower OnFlameThrowerActivate;
};
