// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"

#include "BTTask_GetNextCheckPoint.generated.h"

/**
 * 
 */
UCLASS()
class THISISMYDUNGEON_API UBTTask_GetNextCheckPoint : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

private:

	//	Private Function(s)
	//	---------------------

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

public:

	//	Constructor(s) & Destructor(s)
	//	---------------------------------

	UBTTask_GetNextCheckPoint(FObjectInitializer const& objectInitializer);
};
