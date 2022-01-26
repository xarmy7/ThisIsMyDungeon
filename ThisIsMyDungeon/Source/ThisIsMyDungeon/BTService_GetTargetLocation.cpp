// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_GetTargetLocation.h"

#include "NPCController.h"
#include "Entity.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTService_GetTargetLocation::UBTService_GetTargetLocation(FObjectInitializer const& objectInitializer)
{
	//	Default preview name
	NodeName = TEXT("Get New Location Around Target");
}



void UBTService_GetTargetLocation::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	ANPCController* cont = Cast<ANPCController>(OwnerComp.GetAIOwner());

	if (cont && cont->target)
	{
		//	Retrieve target location and register it in the blackboard key
		OwnerComp.GetBlackboardComponent()->SetValueAsVector(GetSelectedBlackboardKey(), cont->target->GetActorLocation());
	}
}