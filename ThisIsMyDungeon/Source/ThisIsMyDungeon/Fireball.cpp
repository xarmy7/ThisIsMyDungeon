#include "Fireball.h"

#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/SphereComponent.h"
#include "Entity.h"
#include "PlayableCharacter.h"

// Sets default values
AFireball::AFireball()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Use a sphere as a simple collision representation
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->BodyInstance.SetCollisionProfileName(TEXT("Projectile"));
	CollisionComp->InitSphereRadius(10.0f);
	CollisionComp->OnComponentHit.AddDynamic(this, &AFireball::OnHit);		// set up a notification for when this component hits something blocking

	// Players can't walk on it
	CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComp->CanCharacterStepUpOn = ECB_No;

	// Set as root component
	RootComponent = CollisionComp;

	// Use a ProjectileMovementComponent to govern this projectile's movement
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = 0.f;
	ProjectileMovement->MaxSpeed = 7500.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = false;
	ProjectileMovement->ProjectileGravityScale = 1.0f;

	//lol->SetMassOverrideInKg(GetFName(), 1, true);

}

void AFireball::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	Explode();
}


// Called when the game starts or when spawned
void AFireball::BeginPlay()
{
	Super::BeginPlay();

	initialScale = GetActorScale();
}



// Called every frame
void AFireball::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//if (caster == nullptr || caster->isAlive == false) Explode();

	if (charging)
	{
		power = FMath::Min(power + DeltaTime, 2.5f);

		FVector newScale = initialScale * power;

		SetActorScale3D(newScale);
	}
	else
	{

		FVector forward = ProjectileMovement->Velocity * DeltaTime;

		if (CheckRayCast(forward))
		{
			Explode();
			return;
		}

		if (GetLifeSpan() < lifeSpanWhenLaunched * .75f)
		{
			//	Apply bullet drop if bullet is old
			direction.Z -= 0.01f;
			direction.Normalize();
		}
		
		ProjectileMovement->AddForce(direction * 7500.f);
	}
}



bool AFireball::CheckRayCast(const FVector& dir)
{
	FHitResult hit; 

	if (GetWorld()->LineTraceSingleByObjectType(hit, GetActorLocation(), GetActorLocation() + dir, ECollisionChannel::ECC_Pawn))
	{
		if (hit.Actor == this) return false;

		AEntity* entity = Cast<AEntity>(hit.Actor);

		if (entity)
		{
			if(entity == caster) return false;

			return true;
		}
	}
	return false;
}



void AFireball::Launch(const FVector& dir)
{
	direction = dir;

	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	
	charging = false;

	lifeSpanWhenLaunched = lifeSpanWhenLaunched + 1.f * power;
	SetLifeSpan(lifeSpanWhenLaunched);
}


void AFireball::Explode()
{
	//	Ignore self
	TArray<AActor*> ignoreActors;
	ignoreActors.Add(this);

	if (caster)
	{
		ignoreActors.Add(caster);

		APlayableCharacter* player = Cast<APlayableCharacter>(caster);
		if (player && player->currentFireball == this)
		{
			player->StopCharging();
		}
	}

	//	Only collide with Pawn
	TArray<TEnumAsByte<EObjectTypeQuery>> traceObjectTypes;
	traceObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn));

	//	Filter, seek AEntity class
	UClass* seekClass = AEntity::StaticClass();

	TArray<AActor*> overlappedActors;

	UKismetSystemLibrary::SphereOverlapActors(GetWorld(),GetActorLocation() , damageRadius * power, traceObjectTypes, seekClass, ignoreActors, overlappedActors);

	//	Deal damages to all Entities found in the radius 
	for (AActor* currentActor : overlappedActors)
	{
		AEntity* currentEntity = Cast<AEntity>(currentActor);

		if (currentEntity)
		{
			FVector dir = (currentEntity->GetActorLocation() - GetActorLocation()).GetSafeNormal();

			currentEntity->ReceiveDamages(damages * power, dir, knockBackStrength);
		}
	}

	Destroy();
}