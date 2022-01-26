// Fill out your copyright notice in the Description page of Project Settings.


#include "NPCController.h"

#include "NonPlayableCharacter.h"
#include "PATH_Checkpoint.h"
#include "DefenderGameMode.h"

//	Perception
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"

//	AI Behavior tree
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"


ANPCController::ANPCController(const FObjectInitializer& ObjectInitializer) : AAIController(ObjectInitializer)
{
	//	Behavior Tree Setup

	static ConstructorHelpers::FObjectFinder<UBehaviorTree> obj(TEXT("BehaviorTree'/Game/AI/NPC_BehaviorTree.NPC_BehaviorTree'"));

	if (obj.Succeeded())
	{
		BehaviorTree = obj.Object;
	}

	BehaviorTreeComponent = ObjectInitializer.CreateDefaultSubobject<UBehaviorTreeComponent>(this, TEXT("BehaviorTree"));
	BlackboardComponent = ObjectInitializer.CreateDefaultSubobject<UBlackboardComponent>(this, TEXT("BlackBoard Component"));

	//	Sight Setup
	//	Bind controller values to sight sense

	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));
	SetPerceptionComponent(*CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("Perception Component")));

	SightConfig->SightRadius = AISightRadius;
	SightConfig->LoseSightRadius = AILoseSightRadius;
	SightConfig->PeripheralVisionAngleDegrees = AIFieldOfView;
	SightConfig->SetMaxAge(AISightAge);

	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;

	GetPerceptionComponent()->SetDominantSense(*SightConfig->GetSenseImplementation());
	GetPerceptionComponent()->OnPerceptionUpdated.AddDynamic(this, &ANPCController::OnPawnDetected);
	GetPerceptionComponent()->ConfigureSense(*SightConfig);
}



void ANPCController::OnPossess(APawn* inPawn)
{
	Super::OnPossess(inPawn);

	NPCPawn = Cast<ANonPlayableCharacter>(inPawn);
	
	if (NPCPawn)
	{
		NPCPawn->OnPossess(this);
	}
	else
	{
		return;
	}

	//	Start Behavior tree

	RunBehaviorTree(BehaviorTree);

	BehaviorTreeComponent->StartTree(*BehaviorTree);
}



void ANPCController::BeginPlay()
{
	Super::BeginPlay();
}



void ANPCController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (NPCPawn == nullptr) return;

	if (NPCPawn->isAlive == false)
	{
		if (stopped == false && BehaviorTreeComponent)
		{
			BehaviorTreeComponent->StopTree();
			ChangeTarget(nullptr);
			stopped = true;
		}

		return;
	}

	if (target)
	{
		//	Check if target is still valid
		if (!IsTargetStillValid())
		{
			//	If not change target to null
			ChangeTarget(nullptr);
		}

		else
		{
			if (Blackboard) Blackboard->SetValueAsBool(FName("HasTarget"), true);

			//	If has target and pass near its current checkpoint switch to the next one ( avoid the npc following the player then go back all the way back to its current checkpoint)
			if (targetedCheckoint && FVector::DistSquared(targetedCheckoint->GetActorLocation(), NPCPawn->GetActorLocation()) < 600.f * 600.f)
			{
				SwitchToNextCheckpoint();
			}
		}
	}
	else if (targetedCheckoint)
	{
		if(Blackboard) Blackboard->SetValueAsBool(FName("HasTarget"), false);
	}


	//	Attack timer

	if (readyToAttack == false)
	{
		elapsedTimeAttack += DeltaTime;

		if (elapsedTimeAttack >= attackCooldown)
		{
			elapsedTimeAttack = 0.0f;
			readyToAttack = true;
		}
	}
}



void ANPCController::SwitchToNextCheckpoint()
{
	if (targetedCheckoint)
	{
		if (targetedCheckoint->nextCheckPoint.Num() > 0)
		{
			targetedCheckoint = targetedCheckoint->GetNextCheckPoint(pathID);
		}

		//	If last checkpoint
		else if(targetedCheckoint->endPoint == true)
		{
			ChangeTarget(nullptr);

			//	Destroy Enemy for now
			NPCPawn->Kill();


			ADefenderGameMode* GM = Cast<ADefenderGameMode>(GetWorld()->GetAuthGameMode());

			if (GM)
			{
				GM->DealDamagetoDungeon(NPCPawn->damage);
			}
		}
	}
}



void ANPCController::OnPawnDetected(const TArray<AActor*>& DetectedPawn)
{
	if (NPCPawn == nullptr) return;

	if (NPCPawn->isAlive == false) return;

	AEntity* chosenEntity = target; 


	for (AActor* pawn : DetectedPawn)
	{
		AEntity* currentEntity = nullptr;
		currentEntity = Cast<AEntity>(pawn);

		//	If current entity is == to chosen entity, the npc shoukld keep targetting him so return now
		if (target == currentEntity) return;

		//	If cast failed continue to next entity OR if in same team
		if (currentEntity == nullptr || currentEntity->team == NPCPawn->team) continue;

		//	Limit assaillant count to 3, other npc will keep going to their checkPoints
		if (currentEntity->assaillantCount >= 3) continue;

		chosenEntity = currentEntity;
	}

	//	Make chosen entity the new target
	if (chosenEntity == target) return;

	if(chosenEntity) ChangeTarget(chosenEntity);
}



bool ANPCController::IsTargetStillValid()
{
	if (target == nullptr) return false;

	if (target->isAlive == false) return false;

	FActorPerceptionBlueprintInfo Info;

	GetPerceptionComponent()->GetActorsPerception(target, Info);

	if (Info.LastSensedStimuli.Num() > 0)
	{
		const FAIStimulus Stimulus = Info.LastSensedStimuli[0];

		//	Is target still in sight ? 
		if (Stimulus.WasSuccessfullySensed())
		{
			return true;
		}
	}


	if (NPCPawn)
	{
		//	If target still near return true
		if (FVector::DistSquared(NPCPawn->GetActorLocation(), target->GetActorLocation()) <= 400.f * 400.f)
		{
			return true;
		}
	}


	if (attacking) EndAttack();

	return false;
}



void ANPCController::ChangeTarget(class AEntity* newTarget)
{
	if (target)
	{
		//	Decrement Assaillant Count
		target->DecrementAssaillantCount();

		//	If Attacking end the attack
		if (attacking) EndAttack();
	}

	//	CHANGE Target happen HERE
	target = newTarget;

	//	Increment Assaillant Count of the new target if not null
	if(target) target->IncrementAssaillantCount();
}



void ANPCController::Attack()
{
	if (!target || !NPCPawn) return;

	NPCPawn->Attack();
}



void ANPCController::EndAttack()
{
	if (target)
	{
		target->isAttacked = false;
	}

	attacking = false;
}
