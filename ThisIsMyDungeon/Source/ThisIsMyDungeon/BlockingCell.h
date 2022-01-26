// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "BlockingCell.generated.h"

UCLASS()
class THISISMYDUNGEON_API UBlockingCell : public UObject
{
	GENERATED_BODY()

	void CheckIfIsWallBinded(class UWorld* world, float cellSize);
	bool isWallBinded = false;

	UBlockingCell(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	void Setup(const FVector& cellPos, class UWorld* world, float cellSize);

public:

	template <class T>
	static UBlockingCell* Make(T* classT, const FVector& cellPos, class UWorld* world, float cellSize, bool addToRoot)
	{
		UBlockingCell* blockingCell = NewObject<UBlockingCell>(classT);
		blockingCell->Setup(cellPos, world, cellSize);
		
		if (addToRoot)
			blockingCell->AddToRoot();
		
		return blockingCell;
	}


	bool GetIsWallBinded() { return isWallBinded; }
	FVector cellPos;
};