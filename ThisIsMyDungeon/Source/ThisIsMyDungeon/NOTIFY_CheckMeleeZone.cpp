// Fill out your copyright notice in the Description page of Project Settings.


#include "NOTIFY_CheckMeleeZone.h"
#include "Entity.h"
#include "NPCController.h"

void UNOTIFY_CheckMeleeZone::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	AEntity* entity = Cast<AEntity>(MeshComp->GetOwner());

	if (entity)
	{
		entity->CheckMeleeZone();
		
		ANPCController* npccont = Cast<ANPCController>(entity->GetController());

		if (npccont)
		{
			npccont->EndAttack();
		}
	}
}