// Fill out your copyright notice in the Description page of Project Settings.


#include "NonPlayableCharacter.h"

#include "PATH_Checkpoint.h"
#include "DefenderGameMode.h"
#include "NPCController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

ANonPlayableCharacter::ANonPlayableCharacter()
{
	//	Set Default Values

	AIControllerClass = ANPCController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	//	Rotation Settings
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.f, 0.0f);

	//	Team is set to 0 for enemy by default
	team = 0;

	AActor* mesGenoux = UGameplayStatics::GetActorOfClass(GetWorld(), APlayableCharacter::StaticClass());
	me = Cast<APlayableCharacter>(mesGenoux);
}

void ANonPlayableCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void ANonPlayableCharacter::Tick(float DeltaSeconds)
{
	if (isAlive == false)
	{
		elapsedTimeSinceDead += DeltaSeconds;

		if (timeBeforeDeath <= elapsedTimeSinceDead)
		{
			Kill();
		}
		return;
	}

	Super::Tick(DeltaSeconds);

	if (isBurning)
	{
		ReceiveDamages(10.f);
		timerBurning += DeltaSeconds;

		if (timerBurning >= burningTimeMax)
		{
			isBurning = false;
			timerBurning = 0.f;
		}
	}

	//	If lockTarget is true, then force the npc to always face the player
	if (lockTarget)
	{
		if (controller->target)
		{
			FRotator rot = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), controller->target->GetActorLocation());

			SetActorRotation(FRotator(0.0f, rot.Yaw, 0.0f));
		}
		else
		{
			LookAtTarget(false);
		}
	}
}

void ANonPlayableCharacter::OnPossess(ANPCController* in_controller)
{
	//	Get controller ref
	controller = in_controller;
}

void ANonPlayableCharacter::Death()
{
	//	Remove target if dying, Remove target before dying in case the target is locked 
	if (controller)
	{
		controller->ChangeTarget(nullptr);
	}

	if (me != nullptr)
		me->WinManaPoint(manaPointsDrop);

	GetCharacterMovement()->SetActive(false);
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ANonPlayableCharacter::Kill()
{
	if (GetWorld())
	{
		ADefenderGameMode* GM = Cast<ADefenderGameMode>(GetWorld()->GetAuthGameMode());

		if (GM) GM->DecrementEnemyCount();
	}

	Destroy();
}




void ANonPlayableCharacter::ReceiveDamages(int value, const FVector& knockbackDir, const float knockbackStrength, bool ignoreInvulnerability)
{
	if (controller) controller->EndAttack();
	Super::ReceiveDamages(value, knockbackDir, knockbackStrength);
}


void ANonPlayableCharacter::LookAtTarget(const bool enable)
{
	if (enable)
	{
		GetCharacterMovement()->bOrientRotationToMovement = false;
		lockTarget = true;
	}
	else
	{
		GetCharacterMovement()->bOrientRotationToMovement = true;
		lockTarget = false;
	}
}