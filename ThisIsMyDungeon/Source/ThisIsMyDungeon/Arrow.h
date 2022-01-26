// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/CapsuleComponent.h"

#include "Arrow.generated.h"

class UCaspuleComponent;
class UProjectileMovementComponent;


UCLASS()
class THISISMYDUNGEON_API AArrow : public AActor
{
	GENERATED_BODY()

	/** Projectile movement component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	UProjectileMovementComponent* ProjectileMovement;

public:
	// Sets default values for this actor's properties
	AArrow();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/** Sphere collision component */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Settings)
		UCapsuleComponent* CollisionComp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Settings)
		int damage = 25.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Settings, meta = (AllowPrivateAccess = "true"))
	USceneComponent* sceneComp;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/** called when projectile hits something */
	UFUNCTION()
		void OnHit(UPrimitiveComponent* HitComp,
			AActor* OtherActor,
			UPrimitiveComponent* OtherComp,
			FVector NormalImpulse,
			const FHitResult& Hit);

	FVector direction = {0.f, 0.f, 0.f};
};
