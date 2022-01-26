// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "Entity.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnKill);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAttack);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDamaged, const int, currentLife);

UCLASS(config = Game)
class THISISMYDUNGEON_API AEntity : public ACharacter
{
	GENERATED_BODY()
private:

	//	Private Variable(s)
	//	-------------------

	UPROPERTY(VisibleAnyWhere, BlueprintReadOnly, Category = "Settings|Life", meta = (AllowPrivateAccess = "true"))
	int currentLife;
	
	float elapsedTimeInvulnerable = 0.f;

public:

	//	Constructor(s) & Destructor(s)
	//	---------------------------------

	AEntity();

	//	Public Variable(s)
	//	-------------------


	UPROPERTY(VisibleAnyWhere, BlueprintReadOnly, Category = "Settings|Life")
	bool isAlive = true;

	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "Settings|Life")
	int maxLife = 100;

	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "Settings|Life")
	float invulnerabilityTime = .5f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Info")
	bool isInvulnerable = false;

	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "Settings|Combat")
	int damage = 10;

	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "Settings|Combat")
	float meleeZoneRadius = 50.f;

	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "Settings|Combat")
	FVector meleeZonePosition = { 70.f, 00.0f, 10.f };

	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = Settings)
	int team = 0;	//	 0 for player team, 1 for enemy's

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Info")
	int assaillantCount = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Info")
	bool isAttacked = false;

	UPROPERTY(BlueprintAssignable, BlueprintReadWrite, Category = "Settings|Life")
	FOnKill OnKill;

	UPROPERTY(BlueprintAssignable, Category = "Settings|Life")
	FOnDamaged OnDamaged;

	UPROPERTY(BlueprintAssignable, BlueprintReadWrite, Category = Settings)
	FOnAttack OnAttack;

	//	Public Function(s)
	//	-------------------

	virtual void Tick(float DeltaTime) override;

	void CheckMeleeZone();

	virtual  void ReceiveDamages(int value, const FVector& knockbackDir = FVector(0.f, 0.f, 1.f),
		const float knockbackStrength = 0.0f, bool ignoreInvulnerability = false);

	void Attack();

	void IncrementAssaillantCount();
	void DecrementAssaillantCount();

protected:

	//	Protected Variable(s)
	//	-----------------------

	void DealDamageTo(AEntity* receiver);

	virtual void Death() {};

	virtual void BeginPlay() override;
};
