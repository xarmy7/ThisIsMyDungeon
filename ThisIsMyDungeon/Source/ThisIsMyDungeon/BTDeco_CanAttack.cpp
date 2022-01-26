// Fill out your copyright notice in the Description page of Project Settings.

#include "BTDeco_CanAttack.h"

#include "NPCController.h"
#include "Entity.h"


UBTDeco_CanAttack::UBTDeco_CanAttack(FObjectInitializer const& objectInitializer)
{
	//	Default preview name
	NodeName = TEXT("Can Attack ?");
}



bool UBTDeco_CanAttack::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	ANPCController* cont = Cast<ANPCController>(OwnerComp.GetAIOwner());

	//	If cont is not null, its target too and is ready to attack
	if (cont && cont->target && cont->readyToAttack)
	{
		if (cont->target->isAttacked == false)
		{
			//	The target is now attacked by the owner (lock the target)
			cont->target->isAttacked = true; 

			cont->attacking = true;
			cont->readyToAttack = false;

			//	Can Attack
			return true;
		}
	}

	//	Can't Attack
	return false;
}