// Fill out your copyright notice in the Description page of Project Settings.


#include "NOTIFY_Shoot.h"
#include "PlayableCharacter.h"

void UNOTIFY_Shoot::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	APlayableCharacter* player = Cast<APlayableCharacter>(MeshComp->GetOwner());

	if (player)
	{
		player->LaunchFireball();
	}
}