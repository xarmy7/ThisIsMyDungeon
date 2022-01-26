// Fill out your copyright notice in the Description page of Project Settings.


#include "Anvil.h"
#include "Components/BoxComponent.h"
#include "DrawDebugHelpers.h"
#include "Kismet/KismetMathLibrary.h"
#include "NonPlayableCharacter.h"
#include "GenericPlatform/GenericPlatformMath.h"

AAnvil::AAnvil()
{
	meshDamage = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh Damage"));
	meshDamage->AttachToComponent(GetSceneComponent(), FAttachmentTransformRules::KeepRelativeTransform);
	meshDamage->SetCollisionProfileName("OverlapAll", false);

	hitBoxDamage = CreateDefaultSubobject<UBoxComponent>(TEXT("HitBox Damage"));
	hitBoxDamage->AttachToComponent(meshDamage, FAttachmentTransformRules::KeepRelativeTransform);

	detectionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Detection Box"));
	detectionBox->AttachToComponent(GetSceneComponent(), FAttachmentTransformRules::KeepRelativeTransform);

	UStaticMesh* staticMesh = meshDamage->GetStaticMesh();

	if (staticMesh)
		hitBoxDamage->SetBoxExtent(staticMesh->GetBoundingBox().GetExtent());

	refStart = meshDamage->GetComponentLocation();

	placeZone = PlaceZone::Roof;

	hitBoxDamage->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AAnvil::BeginPlay()
{
	Super::BeginPlay();

	hitBoxDamage->OnComponentBeginOverlap.AddDynamic(this, &AAnvil::BeginOverlap);
	detectionBox->OnComponentBeginOverlap.AddDynamic(this, &AAnvil::BeginOverlapDetection);
	detectionBox->OnComponentEndOverlap.AddDynamic(this, &AAnvil::EndOverlapDetection);

	direction = GetActorUpVector();

	wasPlaced = true;
}

void AAnvil::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TraceLine(); //Calculate the distance between the collision and the start

	if (targetNumber > 0)
	{
		activate = true;
	}

	if(activate)
	{
		if (!goUp && FMath::Abs(hitLoc.Z - meshDamage->GetComponentLocation().Z) > near)
		{
			hitBoxDamage->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
			meshDamage->SetWorldLocation(meshDamage->GetComponentLocation() + GetActorUpVector() * speed_ * DeltaTime);
		}
		else
		{
			goUp = true;
			hitBoxDamage->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}

		if (!goUp && refDistance >= distance/divided)
		{
			speed_ = speed_ * 2.f;
			divided -= 2.f;
		}

		if (goUp && refDistance < distance)
		{
			goUp = false;
			activate = false;
		}

		if (goUp)
		{
			meshDamage->SetWorldLocation(meshDamage->GetComponentLocation() + (GetActorUpVector() * -1.f) * speed_/2.f * DeltaTime);
		}
	}
}

void AAnvil::TraceLine()
{
	FHitResult OutHit;

	FVector startLineTrace = meshDamage->GetComponentLocation();

	FVector UpVector = GetActorUpVector();
	FVector endLineTrace = ((UpVector * 5000.f) + startLineTrace);
	FCollisionObjectQueryParams query = FCollisionObjectQueryParams::AllStaticObjects;

	if (GetWorld()->LineTraceSingleByObjectType(OutHit, startLineTrace, endLineTrace, query))
	{
		hitLoc = OutHit.GetComponent()->GetComponentLocation();
		distance = FVector::Dist(hitLoc, startLineTrace);
	}

	if (wasPlaced)
	{
		refDistance = distance;
		wasPlaced = false;
	}

	myVector = hitLoc - meshDamage->GetComponentLocation();
}

void AAnvil::BeginOverlap(UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult) //Damage hitBox
{
	if (hitBoxDamage == nullptr)
		return;

	class ANonPlayableCharacter* enemy = Cast<ANonPlayableCharacter>(OtherActor);
	if (enemy != nullptr)
	{
		enemy->ReceiveDamages(damage, FVector(0.f, 100.f, 0.f), 0.f);
	}
}

void AAnvil::BeginOverlapDetection(UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult) //Detection hitBox
{
	if (detectionBox == nullptr)
		return;

	class ANonPlayableCharacter* enemy = Cast<ANonPlayableCharacter>(OtherActor);
	if (enemy != nullptr)
	{
		targetNumber++;
	}
}

void AAnvil::EndOverlapDetection(UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex)
{
	if (hitBox == nullptr)
		return;

	class ANonPlayableCharacter* enemy = Cast<ANonPlayableCharacter>(OtherActor);
	if (enemy != nullptr)
	{
		targetNumber--;
	}
}
