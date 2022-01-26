// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "PATH_Checkpoint.generated.h"

UCLASS()
class THISISMYDUNGEON_API APATH_Checkpoint : public AActor
{
	GENERATED_BODY()
	
protected:

	//	Protected Function(s)
	//	------------------------

	virtual void BeginPlay() override;

public:	

	//	Constructor(s) & Destructor(s)
	//	---------------------------------

	APATH_Checkpoint();


	//	Public Variable(s)
	//	-------------------
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool endPoint;

	class UBillboardComponent* billboard;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<int, APATH_Checkpoint*> nextCheckPoint;


	//	Public Function(s)
	//	---------------------

	virtual void OnConstruction(const FTransform& Transform) override;

	virtual void Tick(float DeltaTime) override;

	APATH_Checkpoint* GetNextCheckPoint(const int pathID = 0);

	void DrawPath(const FColor& color = FColor::Red, const int pathID = 0);
	
	FVector GetRandomPosInRadius(float radius);
};
