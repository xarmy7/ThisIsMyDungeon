// Fill out your copyright notice in the Description page of Project Settings.


#include "Ice.h"
#include "NonPlayableCharacter.h"
#include "PlayableCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

AIce::AIce()
{
	hitBox->OnComponentBeginOverlap.AddDynamic(this, &AIce::BeginOverlap);
	hitBox->OnComponentEndOverlap.AddDynamic(this, &AIce::EndOverlap);
}

void AIce::BeginOverlap(UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	//APlayableCharacter* playableCharacter = Cast<APlayableCharacter>(OtherActor);
	//
	//if (playableCharacter)
	//{
	//	//playableCharacter->GetCharacterMovement()->GroundFriction = groundFriction;
	//	//playableCharacter->GetCharacterMovement()->BrakingDecelerationWalking = brakingDecelerationWalking;
	//	playableCharacter->GetCharacterMovement()->Velocity =
	//		playableCharacter->GetCharacterMovement()->Velocity * 50;
	//}
	//else
	//{
	//	ANonPlayableCharacter* nonPlayableCharacter = Cast<ANonPlayableCharacter>(OtherActor);
	//
	//	if (!nonPlayableCharacter)
	//		return;
	//
	//	nonPlayableCharacter->GetCharacterMovement()->Velocity = 
	//		nonPlayableCharacter->GetCharacterMovement()->Velocity * 50;
	//}

}

void AIce::EndOverlap(UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex)
{
	//ANonPlayableCharacter* nonPlayableCharacter = Cast<ANonPlayableCharacter>(OtherActor);
	//
	//if (!nonPlayableCharacter)
	//	return;
	//
	//nonPlayableCharacter->DeactiveDrift();
}