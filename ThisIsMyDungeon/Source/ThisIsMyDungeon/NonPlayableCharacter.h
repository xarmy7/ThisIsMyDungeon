// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Entity.h"
#include "PlayableCharacter.h"
#include "NonPlayableCharacter.generated.h"

/**
 * 
 */
UCLASS()
class THISISMYDUNGEON_API ANonPlayableCharacter : public AEntity
{
	GENERATED_BODY()

private:

	//	Private Variable(s)
	//	--------------------

	bool lockTarget = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Settings, meta = (AllowPrivateAccess = "true"))
	int manaPointsDrop;

protected:

	//	Protected Function(s)
	//	------------------------

	virtual void BeginPlay() override; 

	virtual void Death() override;

public:

	//	Constructor(s) & Destructor(s)
	//	---------------------------------
	
	ANonPlayableCharacter();

	APlayableCharacter* me = nullptr;

	//	Public Variable(s)
	//	-------------------

	int pathID;
	int burningTimeMax = 3;

	float timerBurning = 0.f;

	float elapsedTimeSinceDead = 0.f;
	float timeBeforeDeath = 3.f;

	bool isBurning = false;

	class ANPCController* controller = nullptr;

	//	Public Function(s)
	//	-------------------

	void Tick(float DeltaSeconds) override;

	void OnPossess(ANPCController* in_controller);

	void Kill();

	void LookAtTarget(const bool enable = true);


	virtual  void ReceiveDamages(int value, const FVector& knockbackDir = FVector(0.f, 0.f, 1.f),
		const float knockbackStrength = 0.0f, bool ignoreInvulnerability = false) override;
};
