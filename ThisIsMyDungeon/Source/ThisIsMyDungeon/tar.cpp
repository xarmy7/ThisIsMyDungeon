// Fill out your copyright notice in the Description page of Project Settings.


#include "Tar.h"
#include "NonPlayableCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"


ATar::ATar()
{
	hitBox->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	placeZone = PlaceZone::Ground;
}

// Called when the game starts or when spawned
void ATar::BeginPlay()
{
	Super::BeginPlay();

	hitBox->OnComponentBeginOverlap.AddDynamic(this, &ATar::BeginOverlap);
	hitBox->OnComponentEndOverlap.AddDynamic(this, &ATar::EndOverlap);
}

// Called every frame
void ATar::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ATar::BeginOverlap(UPrimitiveComponent* OverlappedComponent,
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
		enemy->GetCharacterMovement()->MaxWalkSpeed /= slowSpeed;
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("IN"));
	}
}

void ATar::EndOverlap(UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex)
{
	if (hitBox == nullptr)
		return;

	class ANonPlayableCharacter* enemy = Cast<ANonPlayableCharacter>(OtherActor);
	if (enemy != nullptr)
	{
		enemy->GetCharacterMovement()->MaxWalkSpeed *= slowSpeed;
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("OUT"));
	}
}