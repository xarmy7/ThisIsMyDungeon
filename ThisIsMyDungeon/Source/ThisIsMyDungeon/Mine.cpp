// Fill out your copyright notice in the Description page of Project Settings.


#include "Mine.h"

AMine::AMine()
{
	targetNumber = 0;
	repetitionDelay = tickRate;
	canExplose = true;

	//sphereHit = CreateDefaultSubobject<USphereComponent>(TEXT("StaticMeshComponentSphere"));
	//sphereHit->AttachToComponent(sceneComponent, FAttachmentTransformRules::KeepRelativeTransform);

	//sphereHit = Cast<USphereComponent>(hitBoxV2);
}

void AMine::BeginPlay()
{
	Super::BeginPlay();

	hitBoxV2->OnComponentBeginOverlap.AddDynamic(this, &AMine::BeginOverlap);
	hitBoxV2->OnComponentEndOverlap.AddDynamic(this, &AMine::EndOverlap);
}

void AMine::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (activate)
	{
		if (canExplose)
		{
			ExplosionDamage(damage);

			if (OnExplose.IsBound()) OnExplose.Broadcast();

			canExplose = false;
			GetWorldTimerManager().SetTimer(timerHandle, this, &AMine::ReloadingFunction, repetitionDelay);
		}
	}
}

void AMine::ReloadingFunction()
{
	canExplose = true;
}

void AMine::ExplosionDamage(float value)
{
	for (int i = enemies.Num() - 1; i >= 0; i--)
	{
		if (enemies[i] == nullptr)
			enemies.Remove(enemies[i]);
			//enemyNull.Add(i);
		else
			enemies[i]->ReceiveDamages(value, enemies[i]->GetActorLocation() - GetActorLocation(), 10.f);
	}
}

void AMine::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	const FHitResult& SweepResult)
{
	if (hitBoxV2 == nullptr)
		return;

	class ANonPlayableCharacter* enemy = Cast<ANonPlayableCharacter>(OtherActor);
	if (enemy != nullptr)
	{
		enemies.Add(enemy);
		activate = true;
		targetNumber++;
	}
}

void AMine::EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (hitBoxV2 == nullptr)
		return;

	class ANonPlayableCharacter* enemy = Cast<ANonPlayableCharacter>(OtherActor);
	if (enemy != nullptr)
	{
		enemies.Remove(enemy);
		targetNumber--;

		if (targetNumber <= 0)
			activate = false;
	}
}