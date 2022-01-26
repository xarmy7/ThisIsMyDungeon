// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"

#include "BTService_GetTargetLocation.generated.h"

/**
 * 
 */
UCLASS()
class THISISMYDUNGEON_API UBTService_GetTargetLocation : public UBTService_BlackboardBase
{
	GENERATED_BODY()

protected:

	//	Protected Function(s)
	//	------------------------

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

public:


	//	Constructor(s) & Destructor(s)
	//	---------------------------------

	UBTService_GetTargetLocation(FObjectInitializer const& objectInitializer);

};
