// Fill out your copyright notice in the Description page of Project Settings.


#include "BufferTrap.h"
#include "Kismet/GameplayStatics.h"


ABufferTrap::ABufferTrap()
{

}

void ABufferTrap::BeginPlay()
{
	Super::BeginPlay();

	timer = tickRate;
}

void ABufferTrap::AddTrap()
{
	TArray<AActor*> actors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATrap::StaticClass(), actors);

	for (AActor* actor : actors)
	{
		ATrap* trap = Cast<ATrap>(actor);
		if (trap != nullptr && (trap->GetActorLocation() - GetActorLocation()).Size() < detectDistance)
		{
			buffedTraps.Add(trap);
		}
	}
}

void ABufferTrap::Tick(float DeltaTime)
{
	//	Update Timer
	timer -= DeltaTime;

	//	Every TickTime
	if (timer <= 0.f)
	{
		timer = tickRate;
		AddTrap();
		Buff();
	}
}

void ABufferTrap::Buff()
{
	for (ATrap* trap : buffedTraps)
	{
		if (!trap->buffed || multipleBuff)
		{
			trap->buffed = true;
			trap->AddBuff();
		}
	}
}