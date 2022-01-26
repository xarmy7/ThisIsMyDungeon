// Fill out your copyright notice in the Description page of Project Settings.


#include "Saw.h"
#include "NonPlayableCharacter.h"
//#include "DrawDebugHelpers.h"

ASaw::ASaw()
{
	bladeSupport = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Blade Support"));
	bladeSupport->AttachToComponent(GetSceneComponent(), FAttachmentTransformRules::KeepRelativeTransform);
	bladeSupport->SetCollisionProfileName("NoCollision", false);

	meshDamage = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh Damage"));
	meshDamage->AttachToComponent(bladeSupport, FAttachmentTransformRules::KeepRelativeTransform);
	meshDamage->SetCollisionProfileName("NoCollision", false);

	hitBoxDamage = CreateDefaultSubobject<UBoxComponent>(TEXT("HitBox Damage"));
	hitBoxDamage->AttachToComponent(bladeSupport, FAttachmentTransformRules::KeepRelativeTransform);
	hitBoxDamage->SetCollisionProfileName("OverlapAll", false);

	Offset = FVector();

	UStaticMesh* staticMesh = meshDamage->GetStaticMesh();

	if (staticMesh)
	{
		hitBoxDamage->SetBoxExtent(staticMesh->GetBoundingBox().GetExtent());
	}

	placeZone = PlaceZone::Ground;
}

// Called when the game starts or when spawned
void ASaw::BeginPlay()
{
	Super::BeginPlay();

	hitBoxDamage->OnComponentBeginOverlap.AddDynamic(this, &ASaw::BeginOverlap);
}

void ASaw::SetValue()
{
	Offset = FVector(0.f, 0.f, 0.f);

	FVector hitBoxScale = hitBox->GetScaledBoxExtent();

	if (hitBoxScale.X > hitBoxScale.Y && hitBoxScale.X > hitBoxScale.Z)
		Offset.X = hitBoxScale.X;
	else if (hitBoxScale.Y > hitBoxScale.Z && hitBoxScale.Y > hitBoxScale.X)
		Offset.Y = hitBoxScale.Y;
	else if (hitBoxScale.Z > hitBoxScale.X && hitBoxScale.Z > hitBoxScale.Y)
		Offset.Z = hitBoxScale.Z;


	FQuat actorQuat = GetActorQuat();
	Offset = actorQuat.RotateVector(Offset);


	//	Update Start and End positions
	Start = hitBoxDamage->GetComponentLocation() - Offset;
	End = Start + 2 * Offset;

	TargetPoint = End;

	//	Set direction
	direction = End - Start;
	direction.Normalize();

	setValue = true;
}

// Called every frame
void ASaw::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//Sets Value only the first time to have location
	if (!setValue)
		SetValue();

	// U-turn when reached the edge
	if ((TargetPoint - hitBoxDamage->GetComponentLocation()).Size() < near)
	{
		direction *= -1;

		if (TargetPoint == End)
			TargetPoint = Start;
		else
			TargetPoint = End;
	}
	// Move to the defined direction
	bladeSupport->SetWorldLocation(bladeSupport->GetComponentLocation() + direction * speed_ * DeltaTime);
}

void ASaw::BeginOverlap(UPrimitiveComponent* OverlappedComponent,
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
		FVector dir = (enemy->GetActorLocation() -  bladeSupport->GetComponentLocation()).GetSafeNormal();

		enemy->ReceiveDamages(damage, dir, 50.f);
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("IN"));
	}
}
