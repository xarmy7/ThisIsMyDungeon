// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Decorators/BTDecorator_BlackboardBase.h"

#include "BTDeco_CanAttack.generated.h"

/**
 * 
 */
UCLASS()
class THISISMYDUNGEON_API UBTDeco_CanAttack : public UBTDecorator_BlackboardBase
{
	GENERATED_BODY()

public:

	//	Constructor(s) & Destructor(s)
	//	---------------------------------

	UBTDeco_CanAttack(FObjectInitializer const& objectInitializer);

	//	Public Function(s)
	//	--------------------

	bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
	
};
