// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Trap.h"
#include "BufferTrap.generated.h"

/**
 * 
 */
UCLASS()
class THISISMYDUNGEON_API ABufferTrap : public ATrap
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Settings, meta = (AllowPrivateAccess = "true"))
		float tickRate = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Settings)
		float detectDistance = 200.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Settings)
		bool multipleBuff = false;

	float timer = 0.f;

	TArray<ATrap*> buffedTraps;

	void AddTrap();

	// Buff only affects already placed traps
	void Buff();
public:
	ABufferTrap();
	virtual void Tick(float DeltaTime) override;


	virtual void AddBuff() {};
};