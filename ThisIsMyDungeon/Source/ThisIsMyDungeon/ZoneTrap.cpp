// Fill out your copyright notice in the Description page of Project Settings.


#include "ZoneTrap.h"

AZoneTrap::AZoneTrap()
{
	hitBox->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}

void AZoneTrap::BeginPlay()
{
	Super::BeginPlay();

	//hitBox->OnComponentBeginOverlap.AddDynamic(this, &AZoneTrap::BeginOverlap);
	//hitBox->OnComponentEndOverlap.AddDynamic(this, &AZoneTrap::EndOverlap);
}