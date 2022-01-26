// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Decorators/BTDecorator_BlackboardBase.h"

#include "BTDeco_IsInFightingRadius.generated.h"

/**
 * 
 */
UCLASS()
class THISISMYDUNGEON_API UBTDeco_IsInFightingRadius : public UBTDecorator_BlackboardBase
{
	GENERATED_BODY()

public:

	//	Constructor(s) & Destructor(s)
	//	---------------------------------

	UBTDeco_IsInFightingRadius(FObjectInitializer const& objectInitializer);


	//	Public Variable(s)
	//	-------------------

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float radius = 300.f;

	//	Public Function(s)
	//	--------------------

	bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
};
