// Fill out your copyright notice in the Description page of Project Settings.


#include "NPC_Spawner.h"

#include "DefenderGameMode.h"
#include "PATH_Checkpoint.h"
#include "NonPLayableCharacter.h"
#include "NPCController.h"

#include "DrawDebugHelpers.h"
#include "Components/BillboardComponent.h"

#define DEFAULT_SPAWNCOUNT 3

// Sets default values
ANPC_Spawner::ANPC_Spawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//	Billboard setup

	billboard = CreateDefaultSubobject<UBillboardComponent>(TEXT("DebugBillboard"));

	if (billboard)
	{
		RootComponent = billboard;

		//	Billboard Size

		billboard->ScreenSize = 0.001f;
		billboard->bIsScreenSizeScaled = 1;

		//	Billboard Image

		ConstructorHelpers::FObjectFinder<UTexture2D> image(TEXT("Texture2D'/Engine/EditorResources/Spawn_Point.Spawn_Point'"));
		
		if (image.Object)
		{
			//	Make the image the billboard sprite
			billboard->SetSprite(image.Object);
		}
	}
}



void ANPC_Spawner::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	
	//DrawPath();
}



void ANPC_Spawner::BeginPlay()
{
	Super::BeginPlay();

	GM = Cast<ADefenderGameMode>(GetWorld()->GetAuthGameMode());

	if(GM) GM->RegisterSpawner(this);
}



// Called every frame
void ANPC_Spawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (GM == nullptr || pathStart == nullptr) return;

	if (active)
	{
		//	Spawn NPCs one by one
		if (NPCToSpawn > spawnCount)
		{
			elapsedTimeSinceSpawn += DeltaTime;

			if (elapsedTimeSinceSpawn > spawnTime)
			{
				Spawn();

				elapsedTimeSinceSpawn = 0.0f;
			}
		}
		else
		{
			Deactivate();
		}
	}
	else if (spawnCount != 0)
	{
		spawnCount = 0;
		NPCToSpawn = (level + DEFAULT_SPAWNCOUNT);
	}
}



void ANPC_Spawner::Spawn()
{
	//	Spawn NPC
	ANonPlayableCharacter* newCharacter = GetWorld()->SpawnActor<ANonPlayableCharacter>(npcClass, GetActorTransform());

	if (newCharacter && newCharacter->controller)
	{
		//	Npc Controller information transfer

		newCharacter->controller->pathID = pathID;
		newCharacter->controller->targetedCheckoint = pathStart;

		newCharacter->maxLife += level * 2;
		newCharacter->damage += level * 2;
	}
	
	if (GM) GM->IncrementEnemyCount();

	spawnCount++;
}



void ANPC_Spawner::Activate()
{
	active = true;
}



void ANPC_Spawner::Deactivate()
{
	active = false;

	level++;
}



void ANPC_Spawner::DrawPath()
{
	if (pathStart)
	{
		DrawDebugLine(GetWorld(), GetActorLocation(), pathStart->GetActorLocation(), pathColor, true, -1, '\000', 5.f);
		pathStart->DrawPath(pathColor, pathID);
	}
}