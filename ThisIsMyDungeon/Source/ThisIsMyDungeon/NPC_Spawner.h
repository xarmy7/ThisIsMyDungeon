// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "NPC_Spawner.generated.h"

UCLASS()
class THISISMYDUNGEON_API ANPC_Spawner : public AActor
{
	GENERATED_BODY()

private:

	//	Private Variable(s)
	//	-------------------

	int spawnCount = 0;

	float elapsedTimeSinceSpawn = 0.f;
	
	class ADefenderGameMode* GM = nullptr;

protected:

	//	Protected Function(s)
	//	--------------------

	virtual void BeginPlay() override;

public:	

	//	Constructor(s) & Destrcutor(s)
	//	---------------------------------

	ANPC_Spawner();

	//	Public Variable(s)
	//	-------------------

	class UBillboardComponent* billboard;

	bool active = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class APATH_Checkpoint* pathStart;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class ANonPlayableCharacter> npcClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int NPCToSpawn = 3;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float spawnTime = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int pathID = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int level = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FColor pathColor = FColor::Blue;

	//	Public Function(s)
	//	--------------------

	virtual void OnConstruction(const FTransform& Transform) override;

	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void Activate();

	UFUNCTION()
	void Deactivate();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void Show();

	virtual void Show_Implementation() {};

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void Hide();
	
	virtual void Hide_Implementation() {};

	void Spawn();

	void DrawPath();
};
