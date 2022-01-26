// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_GetLocAroundTarget.h"

#include "NPCController.h"
#include "Entity.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "NavigationSystem.h"

UBTService_GetLocAroundTarget::UBTService_GetLocAroundTarget(FObjectInitializer const& objectInitializer)
{
	//	Default preview name
	NodeName = TEXT("Get New Location Around Target");

	bRestartTimerOnEachActivation = 0;
}



void UBTService_GetLocAroundTarget::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	if (GetWorld())
	{
		//	Retrieve target position
		FVector targetPos = OwnerComp.GetBlackboardComponent()->GetValueAsVector("LocationTarget");
	
		//	Get Nav mesh system
		UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(GetWorld());
		
		if (NavSystem)
		{
			FNavLocation result;

			//	Get a random navigable point in the radius
			if (NavSystem->GetRandomPointInNavigableRadius(targetPos, 300.f, result))
			{
				//	Get director vector from target to the new location
				FVector dir = result.Location - targetPos;
				dir.Normalize();
				
				//	Register ine the blackboard and get a random lenght between 200.0 and 300.0
				OwnerComp.GetBlackboardComponent()->SetValueAsVector(GetSelectedBlackboardKey(), dir * FMath::FRandRange(200.0f, 300.0f));
			}
		}
	}
}

