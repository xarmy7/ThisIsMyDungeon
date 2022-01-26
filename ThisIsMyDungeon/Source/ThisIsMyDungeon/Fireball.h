// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Fireball.generated.h"

class USphereComponent;
class UProjectileMovementComponent;

UCLASS()
class THISISMYDUNGEON_API AFireball : public AActor
{
	GENERATED_BODY()

private:

	//	Private Variable(s)
	//	--------------------

	/** Sphere collision component */
	UPROPERTY(VisibleDefaultsOnly, Category = Projectile)
	USphereComponent* CollisionComp;

	/** Projectile movement component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	UProjectileMovementComponent* ProjectileMovement;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	float power = 1.f;

	
	FVector initialScale;
	
	FVector direction;

protected:

	//	Protected Variable(s)
	//	-----------------------

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:

	//	Constructor(s) & Detructor(s)
	//	--------------------------------
	
	// Sets default values for this actor's properties
	AFireball();

	//	Public Variable(s)
	//	-------------------

	class AEntity* caster;

	bool charging = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	float lifeSpanWhenLaunched = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	int damages = 25;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	float damageRadius = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	int knockBackStrength = 100.f;

	//	Public Function(s)
	//	--------------------
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	bool CheckRayCast(const FVector& dir);

	UFUNCTION()
	void Launch(const FVector& dir);

	/** Returns CollisionComp subobject **/
	USphereComponent* GetCollisionComp() const { return CollisionComp; }

	/** Returns ProjectileMovement subobject **/
	UProjectileMovementComponent* GetProjectileMovement() const { return ProjectileMovement; }

	/** called when projectile hits something */
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION()
	void Explode();
};
