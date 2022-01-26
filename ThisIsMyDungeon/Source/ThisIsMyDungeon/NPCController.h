// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"	
#include "GameFramework/Controller.h"

#include "NPCController.generated.h"

/**
 * 
 */
UCLASS()
class THISISMYDUNGEON_API ANPCController : public AAIController
{
	GENERATED_BODY()

private:

	//	Private Variable(s)
	//	--------------------

	class UBehaviorTree* BehaviorTree;

	class UBehaviorTreeComponent* BehaviorTreeComponent;

	class UBlackboardComponent* BlackboardComponent;

	float elapsedTimeAttack = 0.f;

	bool stopped = false;

	//	Private Function(s)
	//	--------------------

	UFUNCTION()
	void OnPawnDetected(const TArray<AActor*>& DetectedPawn);

protected:

	virtual void BeginPlay() override;

public:

	//	Constructor(s) & Destructor(s)
	//	---------------------------------

	ANPCController(const FObjectInitializer& ObjectInitializer);

	//	Public Variable(s)
	//	-------------------

	class APATH_Checkpoint* targetedCheckoint = nullptr;

	class AEntity* target = nullptr;

	int pathID = 0;

	bool attacking = false;

	float attackCooldown = 2.f;

	bool readyToAttack = false;

	class ANonPlayableCharacter* NPCPawn;

	//	AI Sight

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = AI)
	float AISightRadius = 1000.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = AI)
	float AISightAge = 5.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = AI)
	float AILoseSightRadius = AISightRadius + 250.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = AI)
	float AIFieldOfView = 90.0f;

	class UAISenseConfig_Sight* SightConfig;

	//	Public Function(s)
	//	--------------------

	virtual void Tick(float DeltaTime) override;

	void OnPossess(APawn* inPawn) override;

	void SwitchToNextCheckpoint();

	void ChangeTarget(class AEntity* newTarget);

	void EndAttack();
	
	void Attack();

	bool IsTargetStillValid();
};
