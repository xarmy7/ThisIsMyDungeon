// Fill out your copyright notice in the Description page of Project Settings.

#include "PController.h"

#include "PlayableCharacter.h"
#include "TrapsLayingComponent.h"
#include "GridComponent.h"

APController::APController()
{
	trapsLayingComponent = CreateDefaultSubobject<UTrapsLayingComponent>(TEXT("TrapsLayingComponent"));
	gridComponent = CreateDefaultSubobject<UGridComponent>(TEXT("GridComponent"));

	trapsLayingComponent->SetGridComponent(gridComponent);
}

void APController::OnPossess(APawn* inPawn)
{
	playerCharacter = Cast<APlayableCharacter>(inPawn);

	if (playerCharacter)
	{
		playerCharacter->OnPossess(this);

		trapsLayingComponent->SetPlayableCharacter(playerCharacter);

		if (trapsLayingComponent)
		{
			trapsLayingComponent->ReloadTrapBar(trapsLayingComponent->traps);
		}
	}

	Super::OnPossess(inPawn);

	if (OnPossessed.IsBound()) OnPossessed.Broadcast();
}


void APController::BeginPlay()
{
	Super::BeginPlay();
}


APlayableCharacter* APController::GetPlayerCharacter()
{
	return playerCharacter;
}
