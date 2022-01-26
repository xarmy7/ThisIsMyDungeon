// Fill out your copyright notice in the Description page of Project Settings.

#include "DefenderGameMode.h"

#include "PlayableCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"
#include "SpawnerManager.h"
#include "NPC_Spawner.h"
#include "DeathCamera.h"
#include "Camera/CameraComponent.h"
#include "PController.h"



ADefenderGameMode::ADefenderGameMode()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Blueprints/Player/BP_PlayableCharacter"));

	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	static ConstructorHelpers::FClassFinder<APController> PlayerControllerBPClass(TEXT("/Game/Blueprints/Player/BP_PlayerController"));

	if (PlayerControllerBPClass.Class != NULL)
	{
		PlayerControllerClass = PlayerControllerBPClass.Class;
	}

	spawners = new SpawnerManager();
}



ADefenderGameMode::~ADefenderGameMode()
{
	delete spawners;
}



void ADefenderGameMode::BeginPlay()
{
	Super::BeginPlay();

	waveCount = 1;
}



void ADefenderGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	RespawnTick(DeltaSeconds);


	if (waveOccuring == false)
	{
		//	Choose first random spawner
		if (spawners->ready == false)
		{
			AwakenSpawner();
			spawners->ShowSpawnMarkers();
		}
		else
		{
			elapsedTimesinceEndWave += DeltaSeconds;

			if (elapsedTimesinceEndWave >= waveDelay)
			{
				elapsedTimesinceEndWave = 0.f;
				StartWave();
			}
		}
	}
	else
	{
		if (waveEnemyCount == 0 && spawners->SpawnerAreInactive())
		{
			//	if next wave is a multiple of 5
			if (waveCount % 5 == 0)
			{
				AwakenSpawner();
			}

			waveOccuring = false;
			waveCount++;

			spawners->ShowSpawnMarkers();
		}
	}
}



void ADefenderGameMode::StartWave()
{
	if (spawners->ready)
	{
		spawners->HideSpawnMarkers();

		if (OnNewWave.IsBound()) OnNewWave.Broadcast();

		waveOccuring = true;
	}
}



void ADefenderGameMode::RegisterSpawner(class ANPC_Spawner* spawner)
{
	if (spawner == nullptr) return;

	spawners->sleepingSpawners.Add(spawner);
}



void ADefenderGameMode::IncrementEnemyCount()
{
	waveEnemyCount++;
}



void ADefenderGameMode::DecrementEnemyCount()
{
	waveEnemyCount--;

	if (waveEnemyCount <= 0)
	{
		waveEnemyCount = 0;
	}
}



void ADefenderGameMode::DealDamagetoDungeon(const int damages)
{
	dungeonLife = FMath::Max(dungeonLife - damages, 0);

	if (dungeonLife <= 0)
	{
		GameOver();
	}
	
	if (OnDungeonLifeChanged.IsBound()) OnDungeonLifeChanged.Broadcast(dungeonLife);
}



void ADefenderGameMode::AwakenSpawner()
{
	ANPC_Spawner* spawner = spawners->AwakenNewSpawner();

	if (spawner)
	{
		OnNewWave.AddDynamic(spawner, &ANPC_Spawner::Activate);
	}
}



void ADefenderGameMode::GameOver()
{
	OnDungeonDestroyed.Broadcast();
}



void ADefenderGameMode::RespawnTick(float DeltaSeconds)
{
	TArray<int> garbageCollect;
	for (TPair<int, float>& player : respawnQueue)
	{
		player.Value += DeltaSeconds;

		if (player.Value >= PlayerRespawnTime)
		{
			garbageCollect.Add(player.Key);
			APlayerController* cont = UGameplayStatics::GetPlayerController(GetWorld(), player.Key);

			if (cont)
			{
				cont->bAutoManageActiveCameraTarget = true;
				//cont->GetPawn()->Destroy();
				RestartPlayer(cont);
			}
		}
	}

	for (int i : garbageCollect)
	{
		respawnQueue.Remove(i);
	}
}



void ADefenderGameMode::RegisterDeathCamera(class ADeathCamera* in_camera)
{
	if(in_camera) deathCamera = in_camera;
}


void ADefenderGameMode::PlayerDeath(const int playerID)
{
	if (respawnQueue.Contains(playerID) == false)
	{
		respawnQueue.Add(playerID, 0.f);
	}

	APlayerController* cont = UGameplayStatics::GetPlayerController(GetWorld(),playerID);

	if (cont && deathCamera)
	{
		cont->SetViewTarget(deathCamera);
	}
}