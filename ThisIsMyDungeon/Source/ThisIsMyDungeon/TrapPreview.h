// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TrapPreview.generated.h"

UCLASS()
class THISISMYDUNGEON_API ATrapPreview : public AActor
{
	GENERATED_BODY()

	class USceneComponent* sceneComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Settings, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* staticMeshComponent;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	UMaterialInterface* materialTrapPreview;

	UMaterialInstanceDynamic* dynMaterial;

public:	
	ATrapPreview();
	void Setup(class UStaticMesh* staticMesh, const FVector& relativeMeshPos, const FVector& relativeScale);

	void Placable(bool value);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
