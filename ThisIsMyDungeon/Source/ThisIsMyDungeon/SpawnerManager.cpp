// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawnerManager.h"
#include "NPC_Spawner.h"



SpawnerManager::SpawnerManager()
{
}



SpawnerManager::~SpawnerManager()
{
}



ANPC_Spawner* SpawnerManager::AwakenNewSpawner()
{
	if (sleepingSpawners.Num() == 0) return nullptr;
	
	int32 chosenSpawner = 0;
	
	if (sleepingSpawners.Num() > 1)
	{
		chosenSpawner = FMath::RandRange(0, sleepingSpawners.Num() - 1);
	}

	sleepingSpawners[chosenSpawner]->level = 1;

	awakenedSpawners.Add(sleepingSpawners[chosenSpawner]);
	sleepingSpawners.RemoveAt(chosenSpawner);

	ready = true;

	return awakenedSpawners[awakenedSpawners.Num() - 1];
}



bool SpawnerManager::SpawnerAreInactive()
{
	for (ANPC_Spawner* s : awakenedSpawners)
	{
		if (s->active)
		{
			return false;
		}
	}
	return true;
}



void SpawnerManager::HideSpawnMarkers()
{
	for (ANPC_Spawner* s : awakenedSpawners)
	{
		if (s)
		{
			s->Hide();
		}
	}
}



void SpawnerManager::ShowSpawnMarkers()
{
	for (ANPC_Spawner* s : awakenedSpawners)
	{
		if (s)
		{
			s->Show();
		}
	}
}