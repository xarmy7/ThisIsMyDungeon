// Fill out your copyright notice in the Description page of Project Settings.


#include "TrapsLayingComponent.h"
#include "PlayableCharacter.h"
#include "GridComponent.h"
#include "Trap.h"
#include "Camera/CameraComponent.h"
#include "DrawDebugHelpers.h"
#include "BlockingCell.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "TrapPreview.h"
#include "Engine/CollisionProfile.h"

UTrapsLayingComponent::UTrapsLayingComponent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PrimaryComponentTick.bCanEverTick = true;

	layingDistanceMax = 1000;
}


void UTrapsLayingComponent::SetupTrapsManagerInput(UInputComponent* PlayerInputComponent)
{
	PlayerInputComponent->BindAction("LMB", IE_Pressed, this, &UTrapsLayingComponent::PlaceTrap);
	PlayerInputComponent->BindAction("RMB", IE_Pressed, this, &UTrapsLayingComponent::SetActiveTrapLaying);

	PlayerInputComponent->BindAction("RotateTrap", IE_Pressed, this, &UTrapsLayingComponent::RotateTrap);

	PlayerInputComponent->BindAction<FInputTrap>("TrapLayingIndexDown", IE_Pressed,
		this, &UTrapsLayingComponent::TrapLayingIndex, -1);

	PlayerInputComponent->BindAction<FInputTrap>("TrapLayingIndexUp", IE_Pressed,
		this, &UTrapsLayingComponent::TrapLayingIndex, 1);
}

void UTrapsLayingComponent::SetPlayableCharacter(APlayableCharacter* _playableCharacter)
{
	playableCharacter = _playableCharacter;
}

void UTrapsLayingComponent::SetGridComponent(UGridComponent* _gridComponent)
{
	gridComponent = _gridComponent;
}

void UTrapsLayingComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UTrapsLayingComponent::ReloadTrapBar(TArray<TSubclassOf<class ATrap>> _traps)
{
	laying = false;

	for (int i = 0; i < trapsLayingInstantiated.Num(); i++)
		GetWorld()->DestroyActor(trapsLayingInstantiated[i]);

	traps.Empty();
	trapsLayingInstantiated.Empty();

	traps = _traps;

	FActorSpawnParameters spawnParameters;
	for (int i = 0; i < traps.Num(); i++)
	{
		AActor* trapActor = GetWorld()->SpawnActor(trapPreview.Get());

		ATrapPreview* trapPreviewInstantiated = Cast<ATrapPreview>(trapActor);
		ATrap* trap = traps[i].GetDefaultObject();

		if (trapPreviewInstantiated)
		{
			trapPreviewInstantiated->Setup(trap->GetStaticMesh(),
				trap->GetStaticMeshRelativePos(), trap->GetStaticMeshRelativeScale());

			trapsLayingInstantiated.Add(trapPreviewInstantiated);
		}
		SetTrapLayingInstantiatedVisibility(i, false);
	}

	if (OnTrapBarReloaded.IsBound()) OnTrapBarReloaded.Broadcast();
}


void UTrapsLayingComponent::SetTrapLayingInstantiatedVisibility(int trapIndex, bool value)
{
	if (trapsLayingInstantiated.Num() > trapIndex)
	{
		trapsLayingInstantiated[trapIndex]->SetActorHiddenInGame(!value);
	}
	else
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("ERROR : Wrong index for 'trapsLayingInstantiated'"));
		}
	}
}

void UTrapsLayingComponent::TrapLayingIndex(int value)
{
	if (!laying)
		return;

	if (value > 0)
	{
		currentTrapLayingIndex++;

		if (currentTrapLayingIndex > trapsLayingInstantiated.Num() - 1)
			currentTrapLayingIndex = 0;
	}
	else
	{
		currentTrapLayingIndex--;

		if (currentTrapLayingIndex < 0)
			currentTrapLayingIndex = trapsLayingInstantiated.Num() - 1;
	}

	for (int i = 0; i < trapsLayingInstantiated.Num(); i++)
		SetTrapLayingInstantiatedVisibility(i, false);

	SetTrapLayingInstantiatedVisibility(currentTrapLayingIndex, true);

	//SetIsLaying(true);

	if (OnTrapLayingIndexChanged.IsBound()) OnTrapLayingIndexChanged.Broadcast(currentTrapLayingIndex);
}

void UTrapsLayingComponent::PlaceTrap()
{
	if (!laying)
		return;

	if (traps[currentTrapLayingIndex].GetDefaultObject()->GetTrapCost() > playableCharacter->GetManaPoints())
		return;

	FVector cellPos;
	FHitResult hitResult;
	if (GetCellPosByCameraLineTrace(cellPos, hitResult))
	{
		if (CheckIfTrapsIsPlacable(cellPos, hitResult.ImpactNormal))
		{
			FActorSpawnParameters spawnParameters;
			AActor* trap = GetWorld()->SpawnActor(traps[currentTrapLayingIndex].Get());
			trapsInstantiated.Add(trap);

			trap->SetActorLocation(cellPos);

			FQuat quat = trap->GetActorQuat().FindBetweenNormals(FVector::UpVector, hitResult.ImpactNormal);

			if (rotate)
				quat *= FQuat::MakeFromEuler(FVector(0.f, 0.f, 90.f));

			trap->SetActorRotation(quat);

			ATrap* currentTrap = traps[currentTrapLayingIndex].GetDefaultObject();
			if (currentTrap->GetIsBlocking())
				AddBlockingCellToList(cellPos);

			playableCharacter->LoseManaPoint(currentTrap->GetTrapCost());
		}
	}

}

void UTrapsLayingComponent::RotateTrap()
{
	rotate = !rotate;
}

void UTrapsLayingComponent::SetActiveTrapLaying()
{
	SetIsLaying(!laying);

	if (laying)
	{
		currentTrapLayingIndex = 0;
		SetTrapLayingInstantiatedVisibility(currentTrapLayingIndex, true);
	}
	else
	{
		SetTrapLayingInstantiatedVisibility(currentTrapLayingIndex, false);
		currentTrapLayingIndex = -1;
	}

	if (OnTrapLayingIndexChanged.IsBound()) OnTrapLayingIndexChanged.Broadcast(currentTrapLayingIndex);

}

bool UTrapsLayingComponent::GetCellPosByCameraLineTrace(FVector& cellPos, FHitResult& hitResult)
{
	FVector cameraLocation = playableCharacter->GetFollowCamera()->GetComponentLocation();
	FVector cameraForward = playableCharacter->GetFollowCamera()->GetForwardVector();

	//FCollisionObjectQueryParams query = FCollisionObjectQueryParams::AllObjects;
	//
	////Ignore BlockingTraps Profile
	//query.RemoveObjectTypesToQuery(ECollisionChannel::ECC_GameTraceChannel5);
	//
	////Ignore Player Profile
	//query.RemoveObjectTypesToQuery(ECollisionChannel::ECC_GameTraceChannel2);

	FHitResult checkHit;
	if (!GetWorld()->LineTraceSingleByChannel(checkHit, cameraLocation, cameraLocation + cameraForward * layingDistanceMax,
		ECollisionChannel::ECC_GameTraceChannel7))
		return false;


	if (GetWorld()->LineTraceSingleByChannel(hitResult, cameraLocation,
		cameraLocation + cameraForward * layingDistanceMax, ECollisionChannel::ECC_GameTraceChannel4))
	{
		if (!checkHit.Location.Equals(hitResult.Location, 0.1))
			return false;

		cellPos = gridComponent->GetCellByPos(hitResult.Location, hitResult.ImpactNormal);
		return true;
	}

	return false;
}

void UTrapsLayingComponent::GetBlockingArroundCell(TArray<int>& blockingTrapsCellsIndex, UBlockingCell* blockingCell)
{
	TArray<FVector> arroundCell;

	float cellSize = gridComponent->cellSize;
	FVector cellPos = blockingCell->cellPos;

	arroundCell.Add(FVector(cellPos.X - cellSize, cellPos.Y, cellPos.Z)); //Left
	arroundCell.Add(FVector(cellPos.X + cellSize, cellPos.Y, cellPos.Z)); //Right
	arroundCell.Add(FVector(cellPos.X, cellPos.Y + cellSize, cellPos.Z)); //Up
	arroundCell.Add(FVector(cellPos.X, cellPos.Y - cellSize, cellPos.Z)); //Down
	arroundCell.Add(FVector(cellPos.X - cellSize, cellPos.Y - cellSize, cellPos.Z)); //Down-Left
	arroundCell.Add(FVector(cellPos.X + cellSize, cellPos.Y - cellSize, cellPos.Z)); //Down-Right
	arroundCell.Add(FVector(cellPos.X - cellSize, cellPos.Y + cellSize, cellPos.Z)); //Up-Left
	arroundCell.Add(FVector(cellPos.X + cellSize, cellPos.Y + cellSize, cellPos.Z)); //Up-Right

	bool cellFinded;
	for (int i = 0; i < blockingTrapsCells.Num(); i++)
	{
		cellFinded = false;

		for (int j = 0; j < blockingTrapsCells[i].Num(); j++)
		{
			for (int k = 0; k < arroundCell.Num(); k++)
			{
				if (blockingTrapsCells[i][j]->cellPos.Equals(arroundCell[k], 0.1))
				{
					blockingTrapsCellsIndex.Add(i);

					FVector toDeleteVect = arroundCell[k];
					arroundCell.Remove(toDeleteVect);

					cellFinded = true;
					break;
				}
			}

			if (cellFinded == true)
				break;
		}
	}

	arroundCell.Reset();
}

void UTrapsLayingComponent::JoinBlockingTrapsList(TArray<int> listIndexs)
{
	TArray<UBlockingCell*> joinedList;

	for (int i = 0; i < listIndexs.Num(); i++)
		for (int j = 0; j < blockingTrapsCells[listIndexs[i]].Num(); j++)
			joinedList.Add(blockingTrapsCells[listIndexs[i]][j]);

	TArray<TArray<UBlockingCell*>> arrayToDelete;
	for (int i = 0; i < listIndexs.Num(); i++)
		arrayToDelete.Add(blockingTrapsCells[listIndexs[i]]);

	for (int i = 0; i < arrayToDelete.Num(); i++)
		blockingTrapsCells.Remove(arrayToDelete[i]);

	blockingTrapsCells.Add(joinedList);
}

void UTrapsLayingComponent::AddBlockingCellToList(const FVector& cellPos)
{

	UBlockingCell* blockingCell = UBlockingCell::Make(this, cellPos, GetWorld(), gridComponent->cellSize, true);

	TArray<int> blockingTrapsCellsIndex;
	GetBlockingArroundCell(blockingTrapsCellsIndex, blockingCell);

	switch (blockingTrapsCellsIndex.Num())
	{
	case 0:
		blockingTrapsCells.Add({ blockingCell });
		break;
	case 1:
		blockingTrapsCells[blockingTrapsCellsIndex[0]].Add(blockingCell);

	default:
		JoinBlockingTrapsList(blockingTrapsCellsIndex);
		blockingTrapsCells[blockingTrapsCells.Num() - 1].Add(blockingCell);
		break;
	}
}

bool UTrapsLayingComponent::CheckWallBindedInBlockingTraps(int listIndex)
{
	for (int i = 0; i < blockingTrapsCells[listIndex].Num(); i++)
	{
		if (blockingTrapsCells[listIndex][i]->GetIsWallBinded())
			return true;
	}

	return false;
}

bool UTrapsLayingComponent::CheckIfTrapsIsPlacable(const FVector& cellPos, const FVector& normal)
{
	ATrap* currentTrap = traps[currentTrapLayingIndex].GetDefaultObject();

	//Check placeZone
	switch (currentTrap->placeZone)
	{
	case PlaceZone::Ground:
		if (normal.Z != 1) return false;
		break;
	case PlaceZone::Roof:
		if (normal.Z != -1) return false;
		break;
	case PlaceZone::Wall:
		if (normal.Z != 0) return false;
		break;
	default:
		break;
	}

	//Check if trap block the path
	#pragma region BlockingTraps

	if (currentTrap->GetIsBlocking())
	{
		UBlockingCell* blockingCell = UBlockingCell::Make(this, cellPos, GetWorld(), gridComponent->cellSize, false);

		int wallBindedCount = 0;

		if (blockingCell->GetIsWallBinded())
			wallBindedCount++;

		TArray<int> listIndexs;
		GetBlockingArroundCell(listIndexs, blockingCell);

		for (int i = 0; i < listIndexs.Num(); i++)
		{
			if (CheckWallBindedInBlockingTraps(listIndexs[i]))
				wallBindedCount++;

			if (wallBindedCount >= 2)
				return false;
		}
	}

	#pragma endregion

	UBoxComponent* boxComponent = currentTrap->GetBoxComponent();
	FVector boxSize = boxComponent->GetUnscaledBoxExtent();
	FQuat quat = currentTrap->GetActorQuat().FindBetweenNormals(FVector::UpVector, normal);

	if (rotate)
		quat *= FQuat::MakeFromEuler(FVector(0, 0, 90));

	FVector relativeLocation = quat.RotateVector(boxComponent->GetRelativeLocation());

	//Check if there is a platform below the trap
	TArray<FVector> points;
	points.Add(FVector(boxSize.X / 2, boxSize.Y / 2, 0));
	points.Add(FVector(boxSize.X / 2, -boxSize.Y / 2, 0));
	points.Add(FVector(-boxSize.X / 2, -boxSize.Y / 2, 0));
	points.Add(FVector(-boxSize.X / 2, boxSize.Y / 2, 0));

	FVector dir = quat.RotateVector(FVector::DownVector);


	for (int i = 0; i < points.Num(); i++)
	{
		FVector start = cellPos + quat.RotateVector(points[i]);
		FVector end = start + dir * 10;

		if (!GetWorld()->LineTraceTestByChannel(start + relativeLocation, end,
			ECollisionChannel::ECC_GameTraceChannel4))
			return false;
	}

	//Check if the trap is not in something
	DrawDebugBox(GetWorld(), cellPos + relativeLocation, boxSize, quat, FColor::Yellow);

	FCollisionObjectQueryParams query = FCollisionObjectQueryParams::AllObjects;
	query.RemoveObjectTypesToQuery(ECollisionChannel::ECC_GameTraceChannel9);

	if (!currentTrap->GetIsBlocking())
		query.RemoveObjectTypesToQuery(ECollisionChannel::ECC_GameTraceChannel5);

	if (GetWorld()->OverlapAnyTestByObjectType(cellPos + relativeLocation,
		quat, query,
		FCollisionShape::MakeBox(boxSize)))
		return false;

	return true;
}

void UTrapsLayingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!laying || playableCharacter == nullptr)
		return;

	FVector cellPos;
	FHitResult hitResult;
	if (GetCellPosByCameraLineTrace(cellPos, hitResult))
	{
		SetTrapLayingInstantiatedVisibility(currentTrapLayingIndex, true);

		trapsLayingInstantiated[currentTrapLayingIndex]->SetActorLocation(cellPos);

		FQuat quat = trapsLayingInstantiated[currentTrapLayingIndex]->GetActorQuat().FindBetweenNormals(FVector::UpVector, hitResult.ImpactNormal);

		if (rotate)
			quat *= FQuat::MakeFromEuler(FVector(0, 0, 90));

		trapsLayingInstantiated[currentTrapLayingIndex]->SetActorRotation(quat);

		if (CheckIfTrapsIsPlacable(cellPos, hitResult.ImpactNormal))
			trapsLayingInstantiated[currentTrapLayingIndex]->Placable(true);
		else
			trapsLayingInstantiated[currentTrapLayingIndex]->Placable(false);

	}
	else
	{
		SetTrapLayingInstantiatedVisibility(currentTrapLayingIndex, false);
	}
}

void UTrapsLayingComponent::SetIsLaying(bool value)
{
	laying = value;

	if (OnTrapLayingActivated.IsBound())
		OnTrapLayingActivated.Broadcast(value);
}


void UTrapsLayingComponent::DestroyComponent(bool bPromoteChildren)
{
	for (TArray<UBlockingCell*> blockingCellArray : blockingTrapsCells)
	{
		for (UBlockingCell* blockingCell : blockingCellArray)
		{
			blockingCell->RemoveFromRoot();
		}
	}
}


void UTrapsLayingComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	for (TArray<UBlockingCell*> blockingCellArray : blockingTrapsCells)
	{
		for (UBlockingCell* blockingCell : blockingCellArray)
		{
			blockingCell->RemoveFromRoot();
		}
	}
}

