// Fill out your copyright notice in the Description page of Project Settings.

#include "DeathCamera.h"

#include "DeathCamera.h"
#include "Camera/CameraComponent.h"
#include "DefenderGameMode.h"


// Sets default values
ADeathCamera::ADeathCamera()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
/*	camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	RootComponent = camera;*/
}

// Called when the game starts or when spawned
void ADeathCamera::BeginPlay()
{
	Super::BeginPlay();

	ADefenderGameMode* GM = Cast<ADefenderGameMode>(GetWorld()->GetAuthGameMode());
	
	if (GM)
	{
		GM->RegisterDeathCamera(this);
	}
}

// Called every frame
void ADeathCamera::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
/*

UCameraComponent* ADeathCamera::GetCamera()
{
	return camera;
}*/