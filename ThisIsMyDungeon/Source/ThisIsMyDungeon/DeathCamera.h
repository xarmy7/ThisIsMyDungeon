// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraActor.h"
#include "DeathCamera.generated.h"

UCLASS()
class THISISMYDUNGEON_API ADeathCamera : public ACameraActor
{
	GENERATED_BODY()

private:

	//	Private Variable(s)
	//	--------------------

	//class UCameraComponent* camera;

protected:

	//	Protected Fucntions(s)
	//	------------------------
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	

	//	Constructor(s) & Destructor(s)
	//	---------------------------------

	// Sets default values for this actor's properties
	ADeathCamera();


	//	Public Fucntions(s)
	//	---------------------
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//class UCameraComponent* GetCamera();
};
