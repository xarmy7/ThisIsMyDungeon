// Fill out your copyright notice in the Description page of Project Settings.


#include "PileLauncher.h"
#include "Arrow.h"
#include "NonPlayableCharacter.h"


APileLauncher::APileLauncher()
{
	DectectionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Detect Zone"));
	DectectionBox->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void APileLauncher::BeginPlay()
{
	Super::BeginPlay();

	DectectionBox->OnComponentBeginOverlap.AddDynamic(this, &APileLauncher::BeginOverlap);
	DectectionBox->OnComponentEndOverlap.AddDynamic(this, &APileLauncher::EndOverlap);

	targetNumber = 0;
	timer = tickRate;
	placeZone = PlaceZone::All;
}

// Called every frame
void APileLauncher::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!initialized)
	{
		SetRangeDetectionBox();
		initialized = true;
	}

	//	Update Timer
	timer -= DeltaTime;

	//	Every TickTime
	if (timer <= 0.f)
	{
		//	Uncomment to debug
		//Shoot();

		timer = tickRate;
		if (targetNumber > 0)
			Shoot();
	}
}

void APileLauncher::Shoot()
{
	UWorld* const world = GetWorld();

	FActorSpawnParameters ActorSpawnParams;
	ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	FQuat quat = GetActorQuat().FindBetweenNormals(FVector::UpVector, FVector(1,0,0));
	//quat *= FQuat::MakeFromEuler(FVector(0, 0, 90));

	newArrow = world->SpawnActor<AArrow>(myArrow, GetActorLocation() + spawnDistance * GetActorUpVector(), quat.Rotator(), ActorSpawnParams);
	//newArrow = world->SpawnActor<AArrow>(myArrow, GetActorTransform(), ActorSpawnParams);
	//newArrow->SetActorRotation(;
	newArrow->direction = GetActorUpVector();
}

void APileLauncher::BeginOverlap(UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	if (DectectionBox == nullptr)
		return;

	class ANonPlayableCharacter* enemy = Cast<ANonPlayableCharacter>(OtherActor);
	if (enemy != nullptr)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("IN"));
		targetNumber++;
	}
}

void APileLauncher::EndOverlap(UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex)
{
	if (DectectionBox == nullptr)
		return;

	class ANonPlayableCharacter* enemy = Cast<ANonPlayableCharacter>(OtherActor);
	if (enemy != nullptr)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("OUT"));
		targetNumber--;
	}
}