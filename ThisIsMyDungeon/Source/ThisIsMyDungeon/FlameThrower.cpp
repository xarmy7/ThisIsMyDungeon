// Fill out your copyright notice in the Description page of Project Settings.


#include "FlameThrower.h"
#include "NonPlayableCharacter.h"

AFlameThrower::AFlameThrower()
{
	DectectionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Detect Zone"));
	DectectionBox->AttachToComponent(sceneComponent, FAttachmentTransformRules::KeepRelativeTransform);

	targetNumber = 0;
	reloadTime = attackTime;
	shootDuration = 5.f;
	burningInterval = 0.25f;
}

void AFlameThrower::BeginPlay()
{
	Super::BeginPlay();

	DectectionBox->OnComponentBeginOverlap.AddDynamic(this, &AFlameThrower::BeginOverlap);
	DectectionBox->OnComponentEndOverlap.AddDynamic(this, &AFlameThrower::EndOverlap);

	baseShootDuration = shootDuration;
}

void AFlameThrower::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	static bool activateOld;

	if (!activate)
		reloadTime -= DeltaTime;

	if (targetNumber > 0 && reloadTime <= 0 && !activate)
	{
		activate = true;
		currentBurningTime = burningInterval;
		if (OnFlameThrowerActivate.IsBound()) OnFlameThrowerActivate.Broadcast();
		UE_LOG(LogTemp, Warning, TEXT("ACTIVE CHANGED"));
	}

	if (activate)
	{
		shootDuration -= DeltaTime;
		currentBurningTime += DeltaTime;

		if (targetNumber > 0 && currentBurningTime >= burningInterval)
		{
			Shoot();
			currentBurningTime = 0;
		}

		if (shootDuration <= 0.f)
		{
			activate = false;
			reloadTime = attackTime;
			shootDuration = baseShootDuration;
			enemies.Empty();
		}
	}

	activateOld = activate;
}

void AFlameThrower::Shoot()
{
	for (int i = 0; i < enemies.Num(); i++)
	{
		UE_LOG(LogTemp, Warning, TEXT("DAMAGE"));
	
		if (enemies[i])
			enemies[i]->ReceiveDamages(damage);
	}
}

void AFlameThrower::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	const FHitResult& SweepResult)
{
	if (hitBox == nullptr)
		return;

	class ANonPlayableCharacter* enemy = Cast<ANonPlayableCharacter>(OtherActor);
	if (enemy != nullptr)
	{
		//enemy->ReceiveDamages(damage);
		enemies.Add(enemy);
		targetNumber++;
	}
}

void AFlameThrower::EndOverlap(UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex)
{
	if (hitBox == nullptr)
		return;

	class ANonPlayableCharacter* enemy = Cast<ANonPlayableCharacter>(OtherActor);
	if (enemy != nullptr)
	{
		enemies.Remove(enemy);
		targetNumber--;
	}
}