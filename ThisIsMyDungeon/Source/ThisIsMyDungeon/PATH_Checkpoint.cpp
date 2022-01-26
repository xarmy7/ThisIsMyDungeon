// Fill out your copyright notice in the Description page of Project Settings.


#include "PATH_Checkpoint.h"

#include "DrawDebugHelpers.h"
#include "Components/BillboardComponent.h"

//	NavMesh
#include "NavigationSystem.h"

// Sets default values
APATH_Checkpoint::APATH_Checkpoint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//	Billboard setup

	billboard = CreateDefaultSubobject<UBillboardComponent>(TEXT("Billboard"));

	if (billboard)
	{
		RootComponent = billboard;

		//	Billboard Size

		billboard->ScreenSize = 0.001f;
		billboard->bIsScreenSizeScaled = 1;

		//	Billboard Image

		ConstructorHelpers::FObjectFinder<UTexture2D> image(TEXT("Texture2D'/Engine/EditorResources/Waypoint.Waypoint'"));

		if (image.Object)
		{
			//	Make the image the billboard sprite
			billboard->SetSprite(image.Object);
		}
	}
}



void APATH_Checkpoint::BeginPlay()
{
	Super::BeginPlay();
	
}



void APATH_Checkpoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}



void APATH_Checkpoint::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	/*int i = 0;
	
	//	avoid user to set next checkpoint to SELF (avoid loop)
	for (APATH_Checkpoint* curr : nextCheckPoint)
	{
		if (curr == this) nextCheckPoint[i] = nullptr;
		i++;
	}*/

	for (TPair<int, APATH_Checkpoint*>& curr : nextCheckPoint)
	{
		if (curr.Value == this) nextCheckPoint[curr.Key] = nullptr;
	}
}



APATH_Checkpoint* APATH_Checkpoint::GetNextCheckPoint(const int pathID)
{
	if (nextCheckPoint.Num() > 0)
	{
		if (nextCheckPoint.Contains(pathID))
		{
			return nextCheckPoint[pathID];;
		}
		else if (nextCheckPoint.Contains(0))
		{
			return nextCheckPoint[0];
		}
	}
	return nullptr;
}



void APATH_Checkpoint::DrawPath(const FColor& color, const int pathID)
{
	if (nextCheckPoint.Num() > 0)
	{
		if (nextCheckPoint.Contains(pathID))
		{
			DrawDebugLine(GetWorld(), GetActorLocation(), nextCheckPoint[pathID]->GetActorLocation(), color, true, -1, '\000', 5.f);

			//	Recursively call DrawPath functions
			nextCheckPoint[pathID]->DrawPath(color, pathID);
		}
		else if (nextCheckPoint.Contains(0))
		{
			DrawDebugLine(GetWorld(), GetActorLocation(), nextCheckPoint[0]->GetActorLocation(), color, true, -1, '\000', 5.f);

			//	Recursively call DrawPath functions
			nextCheckPoint[0]->DrawPath(color, pathID);
		}
	}	
}



FVector APATH_Checkpoint::GetRandomPosInRadius(float radius)
{
	if (GetWorld())
	{
		//	Get random position around the checkpoint to avoid NPC to always follow the same path
		UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(GetWorld());

		if (NavSystem)
		{
			FNavLocation result;
			if (NavSystem->GetRandomPointInNavigableRadius(GetActorLocation(), radius, result))
			{
				return result.Location;
			}
		}
	}

	return GetActorLocation();
}