#include "Entity.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/KismetSystemLibrary.h"

AEntity::AEntity()
{
	PrimaryActorTick.bCanEverTick = true;

	currentLife = maxLife;

	//	Rotation settings	

	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;


	//	Set the camera channel to ignore so it won't block the players' camera anymore

	if (GetCapsuleComponent())
	{
		GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	}
	if (GetMesh())
	{
		GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	}
}



void AEntity::BeginPlay()
{
	Super::BeginPlay();
}



void AEntity::ReceiveDamages(int value, const FVector& knockbackDir, const float knockbackStrength, bool ignoreInvulnerability)
{
	if (ignoreInvulnerability == false && isInvulnerable) return;

	currentLife -= value;

	isInvulnerable = true;

	//	Call OnDamaged event binded functions
	if (OnDamaged.IsBound()) OnDamaged.Broadcast(currentLife);


	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("%d"), currentLife));

	//	If there is knockBack
	if (knockbackStrength != 0.0f)
	{
		//	Launch characetr in knockback direction with a certain strength
		LaunchCharacter(knockbackDir * knockbackStrength, false, true);
	}

	//	If supposed to be dead
	if (currentLife <= 0)
	{
		currentLife = 0;

		//	Call OnKill event binded functions
		if (OnKill.IsBound()) OnKill.Broadcast();

		isAlive = false;
		Death();

		return;
	}
}



void AEntity::CheckMeleeZone()
{
	//	Ignore self
	TArray<AActor*> ignoreActors;
	ignoreActors.Add(this);

	//	Only collide with Pawn or Player
	TArray<TEnumAsByte<EObjectTypeQuery>> traceObjectTypes;
	traceObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn));
	traceObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_GameTraceChannel2));

	//	Filter, seek AEntity class
	UClass* seekClass = AEntity::StaticClass();

	TArray<AActor*> overlappedActors;

	//	Set Position of the sphere in world space
	FRotator rot = GetActorForwardVector().Rotation();
	FVector spherePosition =GetActorLocation() + rot.RotateVector(meleeZonePosition) ;

	//	Get overlapping in casted meleeZone radius at defined position
	UKismetSystemLibrary::SphereOverlapActors(GetWorld(), spherePosition, meleeZoneRadius, traceObjectTypes, seekClass, ignoreActors, overlappedActors);

	//	Deal damages to all Entities found in the radius 
	for (AActor* currentActor : overlappedActors)
	{
		AEntity* currentEntity = Cast<AEntity>(currentActor);

		//	If it's an entity + Is not self + is not teammate -> then the current entity receive damages
		if (currentEntity && currentEntity != this && currentEntity->team != team)
		{
			DealDamageTo(currentEntity);
		}
	}
}



void AEntity::DealDamageTo(AEntity* receiver)
{
	if (receiver)
	{
		//	Calculate Knockback direction
		FVector knockBackDir = receiver->GetActorLocation() - GetActorLocation();

		// Remove Height differences
		knockBackDir.Z = 0;

		knockBackDir.Normalize();

		receiver->ReceiveDamages(damage, knockBackDir, 750.f);
	}
}



void AEntity::Attack()
{
	if (OnAttack.IsBound()) OnAttack.Broadcast();
}


void AEntity::IncrementAssaillantCount()
{
	assaillantCount++;
}

void AEntity::DecrementAssaillantCount()
{
	assaillantCount--;
}



void AEntity::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (isInvulnerable == true)
	{
		elapsedTimeInvulnerable += DeltaTime;

		if (elapsedTimeInvulnerable >= invulnerabilityTime)
		{
			elapsedTimeInvulnerable = 0.f;
			isInvulnerable = false;
		}
	}
}


