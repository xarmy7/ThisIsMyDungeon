#include "BlockingCell.h"
#include "GameFramework/Actor.h"
#include "DrawDebugHelpers.h"

UBlockingCell::UBlockingCell(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

}

void UBlockingCell::Setup(const FVector& _cellPos, UWorld* world, float cellSize)
{
	cellPos = _cellPos;

	CheckIfIsWallBinded(world, cellSize);
}

void UBlockingCell::CheckIfIsWallBinded(UWorld* world, float cellSize)
{
	isWallBinded = world->OverlapAnyTestByObjectType(cellPos + FVector(0, 0, 50),
		FQuat::Identity, FCollisionObjectQueryParams::AllStaticObjects,
		FCollisionShape::MakeBox(FVector(cellSize, cellSize, 0)));
	
}
