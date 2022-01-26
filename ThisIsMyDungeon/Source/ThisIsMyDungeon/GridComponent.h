#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GridComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class THISISMYDUNGEON_API UGridComponent : public UActorComponent
{
	GENERATED_BODY()

	//TMap<FVector, class Cell> cells;

public:	
	UGridComponent();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int cellSize;
	
	FVector GetCellByPos(const FVector& pos, const FVector& normal);
	//void GetCellByPos(class Cell& cell, const FVector& pos, const FVector& normal);
	

protected:
	virtual void BeginPlay() override;


public:	
	//virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
