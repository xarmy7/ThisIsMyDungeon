#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "Trap.generated.h"

UENUM(BlueprintType)
enum class PlaceZone : uint8
{
	All = 0 UMETA(DisplayName = "All"),
	Ground = 1 UMETA(DisplayName = "Ground"),
	Wall = 2 UMETA(DisplayName = "Wall"),
	Roof = 3 UMETA(DisplayName = "Roof")
};

UCLASS()
class THISISMYDUNGEON_API ATrap : public AActor
{
	GENERATED_BODY()

public:	
	ATrap();

	int GetTrapCost() { return trapCost; }

	bool GetIsBlocking() { return isBlocking; }
	void SetIsBlocking(bool value) { isBlocking = value; }

	UStaticMesh* GetStaticMesh();
	FVector GetStaticMeshRelativePos();
	FVector GetStaticMeshRelativeScale();
	UBoxComponent* GetBoxComponent();

	UPROPERTY(EditAnywhere, Category = Settings)
	PlaceZone placeZone;

	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Settings, meta = (AllowPrivateAccess = "true"))
		bool buffed = false;

	virtual void AddBuff() {};

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Settings, meta = (AllowPrivateAccess = "true"))
		int trapCost;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Settings, meta = (AllowPrivateAccess = "true"))
		int unlockTrapCost;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Settings, meta = (AllowPrivateAccess = "true"))
		bool isBlocking;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Settings, meta = (AllowPrivateAccess = "true"))
		class UStaticMeshComponent* staticMeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Settings, meta = (AllowPrivateAccess = "true"))
		class USceneComponent* sceneComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Settings, meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* hitBox;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Settings, meta = (AllowPrivateAccess = "true"))
		class USphereComponent* hitBoxV2;

	USceneComponent* GetSceneComponent();
	UStaticMeshComponent* GetStaticMeshComponent();
};
