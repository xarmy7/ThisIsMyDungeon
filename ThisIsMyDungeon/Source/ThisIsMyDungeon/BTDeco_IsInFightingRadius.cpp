// Fill out your copyright notice in the Description page of Project Settings.


#include "BTDeco_IsInFightingRadius.h"

#include "NPCController.h"
#include "NonPlayableCharacter.h"

#include "BehaviorTree/BlackboardComponent.h"


UBTDeco_IsInFightingRadius::UBTDeco_IsInFightingRadius(FObjectInitializer const& objectInitializer)
{
	//	Default preview name
	NodeName = TEXT("Is In Fighting Radius");
}



bool UBTDeco_IsInFightingRadius::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	ANPCController* cont = Cast<ANPCController>(OwnerComp.GetAIOwner());

	if (cont)
	{
		FVector targetPos = OwnerComp.GetBlackboardComponent()->GetValueAsVector(GetSelectedBlackboardKey());
		
		//	If distance between target and pawn is under radius(input), the pawn is in the fighting radius
		if (FVector::DistSquared(targetPos, cont->NPCPawn->GetActorLocation()) <= radius * radius)
		{
			//	Make the pawn look at the target
			if (cont->NPCPawn) cont->NPCPawn->LookAtTarget(true);

			// Is in fighting radius
			return true;
		}
	}

	//	Disable pawn looking at its target
	if (cont->NPCPawn) cont->NPCPawn->LookAtTarget(false);

	// Is Not in fighting radius
	return false;
}