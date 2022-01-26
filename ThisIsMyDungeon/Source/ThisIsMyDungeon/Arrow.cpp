// Fill out your copyright notice in the Description page of Project Settings.


#include "Arrow.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "NonPlayableCharacter.h"

#include "DrawDebugHelpers.h"


// Sets default values
AArrow::AArrow()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComp"));

	RootComponent = CollisionComp;

	CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComp->CanCharacterStepUpOn = ECB_No;

	// Use a ProjectileMovementComponent to govern this projectile's movement
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = 0.f;
	ProjectileMovement->MaxSpeed = 7500.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = false;
	ProjectileMovement->ProjectileGravityScale = 1.0f;
}

// Called when the game starts or when spawned
void AArrow::BeginPlay()
{
	Super::BeginPlay();

	//	Doesn't work
	InitialLifeSpan = 2.f;

	CollisionComp->OnComponentHit.AddDynamic(this, &AArrow::OnHit);
	//	Set up a notification for when this component hits something blocking

	//FVector direction = GetActorUpVector();

	//ProjectileMovement->AddForce(direction * 7500.f);
}

// Called every frame
void AArrow::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	ProjectileMovement->AddForce(direction * 7500.f);

	FHitResult hit;
	
	FVector forward = ProjectileMovement->Velocity * DeltaTime;

	DrawDebugLine(GetWorld(), GetActorLocation(), GetActorLocation() + forward, FColor::Red, false, -1, 0, 2);
	if (GetWorld()->LineTraceSingleByObjectType(hit, GetActorLocation(), GetActorLocation() + forward, ECollisionChannel::ECC_Pawn))
	{
		if (hit.Actor == this) return;

		class ANonPlayableCharacter* enemy = Cast<ANonPlayableCharacter>(hit.Actor);

		if (enemy)
		{
			enemy->ReceiveDamages(damage, FVector(0.f, 0.f, 0.f), 0.f);
		}
		Destroy();
	}
}

void AArrow::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("BOUM"));

	class ANonPlayableCharacter* enemy = Cast<ANonPlayableCharacter>(OtherActor);
	if (enemy != nullptr)
	{
		enemy->ReceiveDamages(damage, FVector(0.f, 0.f, 0.f), 0.f);
	}

	Destroy();
}