// Fill out your copyright notice in the Description page of Project Settings.


#include "GridComponent.h"

// Sets default values for this component's properties
UGridComponent::UGridComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	cellSize = 200;
}

// Called when the game starts
void UGridComponent::BeginPlay()
{
	Super::BeginPlay();
}

FVector UGridComponent::GetCellByPos(const FVector& pos, const FVector& normal)
{
	FVector normalPos = pos * normal.GetAbs();
	FVector cellPos = pos;

	for (int i = 0; i < 3; i++)
	{
		int cell = FMath::FloorToInt(pos[i] / cellSize);
		cellPos[i] = (cell * cellSize + cellSize / 2);
	}

	cellPos -= cellPos * normal.GetAbs();

	return cellPos + normalPos;
}

