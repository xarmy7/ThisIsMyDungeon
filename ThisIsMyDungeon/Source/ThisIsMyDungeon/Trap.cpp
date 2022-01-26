#include "Trap.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"

ATrap::ATrap()
{
	PrimaryActorTick.bCanEverTick = true;

	sceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	SetRootComponent(sceneComponent);

	hitBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionMesh"));
	hitBox->AttachToComponent(sceneComponent, FAttachmentTransformRules::KeepRelativeTransform);

	hitBoxV2 = CreateDefaultSubobject<USphereComponent>(TEXT("StaticMeshComponentV2"));
	hitBoxV2->AttachToComponent(sceneComponent, FAttachmentTransformRules::KeepRelativeTransform);

	staticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	staticMeshComponent->AttachToComponent(sceneComponent, FAttachmentTransformRules::KeepRelativeTransform);
	staticMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

UStaticMesh* ATrap::GetStaticMesh()
{ 
	return staticMeshComponent->GetStaticMesh();
}

FVector ATrap::GetStaticMeshRelativePos()
{
	return staticMeshComponent->GetRelativeLocation();
}

FVector ATrap::GetStaticMeshRelativeScale()
{
	return staticMeshComponent->GetRelativeScale3D();
}

UBoxComponent* ATrap::GetBoxComponent()
{
	return hitBox;
}

USceneComponent* ATrap::GetSceneComponent()
{
	return sceneComponent;
}

void ATrap::BeginPlay()
{
	Super::BeginPlay();
}

void ATrap::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

UStaticMeshComponent* ATrap::GetStaticMeshComponent()
{
	return staticMeshComponent;
}