// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ZoneTrap.h"
#include "Lava.generated.h"

class AEntity;

/**
 *
 */
UCLASS()
class THISISMYDUNGEON_API ALava : public AZoneTrap
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

	void MakeDamage();

	TArray<AEntity*> enemies;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Settings, meta = (AllowPrivateAccess = "true"))
		float tickRate = 1.f;

	float timer = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Settings, meta = (AllowPrivateAccess = "true"))
		float buffValue = 2.f;

public:
	ALava();

	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Settings, meta = (AllowPrivateAccess = "true"))
		float damage = 1.f;

	virtual void AddBuff() override { damage *= buffValue; };

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
};
