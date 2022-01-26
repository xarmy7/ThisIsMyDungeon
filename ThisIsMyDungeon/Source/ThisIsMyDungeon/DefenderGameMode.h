// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "DeathCamera.h"
#include "DefenderGameMode.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDungeonDestroyed);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDungeonLife, const int, dungeonLife);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnNewWave);


/**
 * 
 */
UCLASS()
class THISISMYDUNGEON_API ADefenderGameMode : public AGameModeBase
{
	GENERATED_BODY()

private:

	//	Private Variable(s)
	//	-------------------

	float elapsedTimesinceEndWave = 0.f;

	TMap<int, float> respawnQueue;

	class SpawnerManager* spawners;

	UPROPERTY(VisibleAnyWhere, BlueprintReadOnly, Category = "Wave Settings", meta = (AllowPrivateAccess = "true"))
	int waveCount = 0;

	UPROPERTY(VisibleAnyWhere, BlueprintReadOnly, Category = "Wave Settings", meta = (AllowPrivateAccess = "true"))
	bool waveOccuring = false;

	UPROPERTY(VisibleAnyWhere, BlueprintReadOnly, Category = "Wave Settings", meta = (AllowPrivateAccess = "true"))
	int waveEnemyCount = 0;

	UPROPERTY(VisibleAnyWhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class ADeathCamera* deathCamera;

	//	Private Function(s)
	//	--------------------

	void StartWave();

protected:

	//	Protected Function(s)
	//	------------------------

	virtual void BeginPlay() override;


public:

	//	Constructor(s) & Destructor(s)
	//	---------------------------------

	ADefenderGameMode();
	~ADefenderGameMode();
	
	//	Public Variable(s)
	//	-------------------

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Wave Settings")
	float waveDelay = 5.f;

	UPROPERTY(VisibleAnyWhere, BlueprintReadOnly, Category = "Wave Settings")
	int dungeonLife = 500;

	UPROPERTY(VisibleAnyWhere, BlueprintReadOnly, Category = "Player Settings")
	float PlayerRespawnTime = 5.f;

	UPROPERTY(VisibleAnyWhere, BlueprintAssignable, Category = "Event Dispatcher")
	FOnDungeonDestroyed OnDungeonDestroyed;

	UPROPERTY(VisibleAnyWhere, BlueprintAssignable, Category = "Event Dispatcher")
	FDungeonLife OnDungeonLifeChanged;

	UPROPERTY(VisibleAnyWhere, BlueprintAssignable, Category = "Event Dispatcher")
	FOnNewWave OnNewWave;

	//	Public Function(s)
	//	--------------------

	virtual void Tick(float DeltaSeconds) override;

	void RespawnTick(float DeltaSeconds);

	void IncrementEnemyCount();
	void DecrementEnemyCount();

	void DealDamagetoDungeon(const int damages);
	void GameOver();

	void AwakenSpawner();

	void RegisterDeathCamera(class ADeathCamera* in_camera);

	UFUNCTION()
	void PlayerDeath(const int playerID);

	void RegisterSpawner(class ANPC_Spawner* spawner);
};
