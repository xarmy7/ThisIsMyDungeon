// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BlockingCell.h"
#include "TrapsLayingComponent.generated.h"

DECLARE_DELEGATE_OneParam(FInputTrap, const int);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTrapIndex, const int, index);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTrapActivate, bool, isActivate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FTrapBarReload);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class THISISMYDUNGEON_API UTrapsLayingComponent : public UActorComponent
{
	GENERATED_BODY()

	class APlayableCharacter* playableCharacter;
    class UGridComponent* gridComponent;

	TArray<class ATrapPreview*> trapsLayingInstantiated;

	TArray<TArray<UBlockingCell*>> blockingTrapsCells;

	void GetBlockingArroundCell(TArray<int>& blockingTrapsCellsIndex, UBlockingCell* blockingCell);
	void JoinBlockingTrapsList(TArray<int> listIndexs);
	void AddBlockingCellToList(const FVector& cellPos);
	bool CheckWallBindedInBlockingTraps(int listIndex);

	bool CheckIfTrapsIsPlacable(const FVector& cellPos, const FVector& normal);

	TArray<AActor*> trapsInstantiated;

	int currentTrapLayingIndex = 0;
	bool laying = false;
	bool rotate = false;

	void SetTrapLayingInstantiatedVisibility(int trapIndex, bool value);
	bool GetCellPosByCameraLineTrace(FVector& cellPos, FHitResult& hitResult);

	void SetIsLaying(bool value);

public:	
	UTrapsLayingComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	bool GetLaying() { return laying; }

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Settings)
	TArray<TSubclassOf<class ATrap>> traps;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Settings)
	TSubclassOf<class ATrapPreview> trapPreview;

	UFUNCTION(BlueprintCallable, Category = "Component|Traps")
	void ReloadTrapBar(TArray<TSubclassOf<class ATrap>> _traps);

	UFUNCTION()
	void TrapLayingIndex(int value);
	void SetActiveTrapLaying();
	void PlaceTrap();
	void RotateTrap();

	void SetPlayableCharacter(class APlayableCharacter* playableCharacter);
	void SetGridComponent(class UGridComponent* gridComponent);
	void SetupTrapsManagerInput(class UInputComponent* PlayerInputComponent);

	UFUNCTION(BlueprintCallable)
	int GetCurrentLayingIndex() { return currentTrapLayingIndex; }

	UFUNCTION(BlueprintCallable)
	TArray<TSubclassOf<class ATrap>> GetTraps() { return traps; }

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Settings)
	int layingDistanceMax;

	UPROPERTY(BlueprintAssignable, Category = "Components|Traps")
	FTrapIndex OnTrapLayingIndexChanged;

	UPROPERTY(BlueprintAssignable, Category = "Components|Traps")
	FTrapActivate OnTrapLayingActivated;

	UPROPERTY(BlueprintAssignable, Category = "Components|Traps")
	FTrapBarReload OnTrapBarReloaded;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	virtual void DestroyComponent(bool bPromoteChildren = false) override;
		
};
