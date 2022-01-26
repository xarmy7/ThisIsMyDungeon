// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"

#include "BTTask_GetCheckPointLocation.generated.h"

/**
 * 
 */
UCLASS()
class THISISMYDUNGEON_API UBTTask_GetCheckPointLocation : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

private:
	
	//	Private Function(s)
	//	--------------------

	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

public:

	//	Constructor(s) & Destructor(s)
	//	---------------------------------

	UBTTask_GetCheckPointLocation(FObjectInitializer const& objectInitializer);
	
};
