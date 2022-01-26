// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class THISISMYDUNGEON_API SpawnerManager
{
public:

	//	Constructor(s) & Destructor(s)
	//	----------------------------------

	SpawnerManager();
	~SpawnerManager();

	//	Public Variable(s)
	//	-------------------

	TArray<class ANPC_Spawner*> sleepingSpawners;
	TArray<class ANPC_Spawner*> awakenedSpawners;


	bool ready = false;

	//	Public Function(s)
	//	--------------------

	ANPC_Spawner* AwakenNewSpawner();
	
	bool SpawnerAreInactive();

	void HideSpawnMarkers();

	void ShowSpawnMarkers();
};
