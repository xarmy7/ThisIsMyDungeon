// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_Attack.h"

#include "NPCController.h"


UBTTask_Attack::UBTTask_Attack(FObjectInitializer const& objectInitializer)
{
	//	Default preview name
	NodeName = TEXT("Attack");
}



EBTNodeResult::Type UBTTask_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ANPCController* cont = Cast<ANPCController>(OwnerComp.GetAIOwner());

	if (cont)
	{
		//	Call Attack function of the owner controller
		cont->Attack();

		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}
