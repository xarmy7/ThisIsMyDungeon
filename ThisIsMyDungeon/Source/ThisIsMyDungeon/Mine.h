// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ZoneTrap.h"
#include "NonPlayableCharacter.h"
#include "Components/SphereComponent.h"
#include "Mine.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnExplose);

/**
 * 
 */
UCLASS()
class THISISMYDUNGEON_API AMine : public AZoneTrap
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

	void ReloadingFunction();
	void ExplosionDamage(float value);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Settings, meta = (AllowPrivateAccess = "true"))
		float BuffValue = 2.f;

	FTimerHandle timerHandle;

	USphereComponent* sphereHit;

	TArray<ANonPlayableCharacter*> enemies;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Settings, meta = (AllowPrivateAccess = "true"))
		float tickRate = 5.f;

	float reloadTime = 0.f;
	float repetitionDelay = 0.f;

	int targetNumber = 0;

	bool activate = false;
	bool canExplose = false;

public:
	AMine();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Settings, meta = (AllowPrivateAccess = "true"))
	float damage = 1.f;

	UPROPERTY(BlueprintAssignable, Category = Event)
	FOnExplose OnExplose;

	virtual void Tick(float DeltaTime) override;

	virtual void AddBuff() override { damage *= BuffValue; };

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
