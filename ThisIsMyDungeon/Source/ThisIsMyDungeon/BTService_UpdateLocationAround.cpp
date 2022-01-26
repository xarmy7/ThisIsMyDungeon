// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_UpdateLocationAround.h"

#include "NPCController.h"
#include "Entity.h"
#include "NonPlayableCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"

//	NavMesh
#include "NavigationSystem.h"

UBTService_UpdateLocationAround::UBTService_UpdateLocationAround(FObjectInitializer const& objectInitializer)
{
	//	Default preview name
	NodeName = TEXT("Update Location Around Target");
}



void UBTService_UpdateLocationAround::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	ANPCController* cont = Cast<ANPCController>(OwnerComp.GetAIOwner());

	if (cont)
	{
		if (GetWorld())
		{
			//	Retrieve target position
			FVector targetPos = OwnerComp.GetBlackboardComponent()->GetValueAsVector("LocationTarget");

			//	Get Current direction vector from target to pawn
			FVector currentDir = cont->NPCPawn->GetActorLocation() - targetPos;
			currentDir.Z = 0;

			//	Get targeted direction registered in the blackboard
			FVector targetedDir = OwnerComp.GetBlackboardComponent()->GetValueAsVector("LocationDir");
			targetedDir.Z = 0;

			//	Lerp currentDir to targetedDir
			FVector newDir = FMath::Lerp(currentDir, targetedDir, .25f);
			newDir.Normalize();
			newDir = newDir * targetedDir.Size();

			//	Send the lerped value to the blackboard so the pawn turn around the player
			OwnerComp.GetBlackboardComponent()->SetValueAsVector(GetSelectedBlackboardKey(), targetPos + newDir);
		}
	}
}

