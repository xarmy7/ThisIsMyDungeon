// Fill out your copyright notice in the Description page of Project Settings.


#include "Lava.h"
#include "NonPlayableCharacter.h"


ALava::ALava()
{
	hitBox->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	placeZone = PlaceZone::Ground;
}

// Called when the game starts or when spawned
void ALava::BeginPlay()
{
	Super::BeginPlay();

	hitBox->OnComponentBeginOverlap.AddDynamic(this, &ALava::BeginOverlap);
	hitBox->OnComponentEndOverlap.AddDynamic(this, &ALava::EndOverlap);
	timer = tickRate;
}

// Called every frame
void ALava::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//	Update Timer
	timer -= DeltaTime;

	//	Every TickTime
	if (timer <= 0.f)
	{
		timer = tickRate;
		MakeDamage();
	}
}

void ALava::BeginOverlap(UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	if (hitBox == nullptr)
		return;

	class ANonPlayableCharacter* enemy = Cast<ANonPlayableCharacter>(OtherActor);
	if (enemy != nullptr)
	{
		//entity->GetCharacterMovement()->MaxWalkSpeed /= slowSpeed;
		enemies.Add(enemy);
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("IN"));
	}
}

void ALava::EndOverlap(UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex)
{
	if (hitBox == nullptr)
		return;

	class ANonPlayableCharacter* enemy = Cast<ANonPlayableCharacter>(OtherActor);
	if (enemy != nullptr)
	{
		//entity->GetCharacterMovement()->MaxWalkSpeed *= slowSpeed;
		enemies.Remove(enemy);
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("OUT"));
	}
}

void ALava::MakeDamage()
{
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("entity number is") + FString::SanitizeFloat(entities.Num()));

	for (auto enemy : enemies)
	{
		if (enemy != nullptr)
		{
			//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("Damage Tick"));
			enemy->ReceiveDamages(damage, FVector(0, 0, 0), 0.f);
		}
	}
}