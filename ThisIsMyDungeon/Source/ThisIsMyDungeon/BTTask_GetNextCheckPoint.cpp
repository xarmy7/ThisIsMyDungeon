

#include "BTTask_GetNextCheckPoint.h"

#include "PATH_Checkpoint.h"
#include "NPCController.h"

UBTTask_GetNextCheckPoint::UBTTask_GetNextCheckPoint(FObjectInitializer const& objectInitializer)
{
	NodeName = TEXT("Get Next Checkpoint");
}



EBTNodeResult::Type UBTTask_GetNextCheckPoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ANPCController* cont = Cast<ANPCController>(OwnerComp.GetAIOwner());

	if (cont)
	{
		//	Change CheckPoint to the next
		cont->SwitchToNextCheckpoint();

		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}