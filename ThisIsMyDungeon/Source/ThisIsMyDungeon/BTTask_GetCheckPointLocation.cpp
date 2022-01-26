// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_GetCheckPointLocation.h"

#include "NPCController.h"
#include "PATH_Checkpoint.h"
#include "BehaviorTree/BlackboardComponent.h"


UBTTask_GetCheckPointLocation::UBTTask_GetCheckPointLocation(FObjectInitializer const& objectInitializer)
{
	NodeName = TEXT("Get Checkpoint Location");
}



EBTNodeResult::Type UBTTask_GetCheckPointLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ANPCController* cont = Cast<ANPCController>(OwnerComp.GetAIOwner());

	if (cont && cont->targetedCheckoint)
	{
		//	Get A random location around the targeted checkpoint of the owner
		OwnerComp.GetBlackboardComponent()->SetValueAsVector("CheckPointLocation", cont->targetedCheckoint->GetRandomPosInRadius(100.f));

		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}