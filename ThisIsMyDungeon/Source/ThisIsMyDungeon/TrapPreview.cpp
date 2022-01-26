#include "TrapPreview.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
ATrapPreview::ATrapPreview()
{
	PrimaryActorTick.bCanEverTick = true;

	sceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	SetRootComponent(sceneComponent);

	staticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	staticMeshComponent->AttachToComponent(sceneComponent, FAttachmentTransformRules::KeepRelativeTransform);
	staticMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

}

// Called when the game starts or when spawned
void ATrapPreview::BeginPlay()
{
	Super::BeginPlay();
}

void ATrapPreview::Setup(UStaticMesh* staticMesh, const FVector& relativeMeshPos, const FVector& relativeScale)
{
	staticMeshComponent->SetStaticMesh(staticMesh);
	staticMeshComponent->SetRelativeLocation(relativeMeshPos);
	staticMeshComponent->SetRelativeScale3D(relativeScale);
	
	dynMaterial = UMaterialInstanceDynamic::Create(materialTrapPreview, this);
	staticMeshComponent->SetMaterial(0, dynMaterial);

	Placable(false);
}

void ATrapPreview::Placable(bool value)
{
	if (value)
		dynMaterial->SetVectorParameterValue("color", FColor::Green);
	else
		dynMaterial->SetVectorParameterValue("color", FColor::Red);
}

// Called every frame
void ATrapPreview::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

