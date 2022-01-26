#pragma once

#include "CoreMinimal.h"
#include "Entity.h"
#include "Fireball.h"
#include "PlayableCharacter.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDarkPoint, const int, currentDarkPoint);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerDeath, const int, controllerID);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInTriggerTrapSeller, bool, inTrigger);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnShoot);

UCLASS(config = Game)
class THISISMYDUNGEON_API APlayableCharacter : public AEntity
{
	GENERATED_BODY()

private:

	//	Private Variable(s)
	//	--------------------

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

	UPROPERTY(EditDefaultsOnly, Category = Projectile)
	TSubclassOf<class AFireball> fireballBaseClass;

	/** Location on gun mesh where projectiles should spawn. */
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	USceneComponent* FP_MuzzleLocation;

	FTimerHandle timerHandle;

	FRotator camRotRef;
	FRotator rot;
	FRotator initRotation;

	FVector initScale = {};

	float attackSpeed = 1.0f;
	float time = 0.f;
	float walkSpeed = 0.f;
	float mainTime = 0.f;
	float delay = 5.f;
	float newDelay = 3.f;
	float repetitionDelay = 0.25f;

	int addMaxManaPoint = 10;

	bool canShoot = false;
	bool hasShooted = false;
	bool block = false;

	UPROPERTY(VisibleAnyWhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class APController* playerController;

	//	Private Function(s)
	//	--------------------

protected :

	//	Protected Function(s)
	//	-----------------------

	virtual void BeginPlay() override;

	void MoveForward(float Value);

	void MoveRight(float Value);

	void TurnAtRate(float Rate);

	void LookUpAtRate(float Rate);

	void RepeatingFunction();

	void Charge();

	void Shoot();

	void Sprinting();

	void StopSprinting();

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public :

	//	Constructor(s) & Destructor(s)
	//	---------------------------------

	APlayableCharacter();

	//	Public Variable(s)
	// -------------------

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	float BaseTurnRate;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	float BaseLookUpRate;
	/*
	UPROPERTY(VisibleAnyWhere, BlueprintReadOnly, Category = "TrapsManager")
	class UTrapsLayingComponent* trapsLayingComponent;

	UPROPERTY(VisibleAnyWhere, BlueprintReadOnly, Category = "Grid")
	class UGridComponent* gridComponent;
	*/
	UPROPERTY(BlueprintAssignable, Category = DarkPower)
	FDarkPoint DarkPointChanged;

	UPROPERTY(BlueprintAssignable, Category = "EventDispatcher")
	FOnPlayerDeath OnPlayerDeath;

	UPROPERTY(BlueprintAssignable, Category = "EventDispatcher")
	FOnShoot OnShoot; 

	UPROPERTY(BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class AFireball* currentFireball = nullptr;


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Data)
	bool charging = false;

	//	Public Function(s)
	//	--------------------

	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = Player)
	virtual void Death() override;

	void ChargeFireball();
	
	void StopCharging();

	void LaunchFireball();

	void OnPossess(class APController* controller);


	UPROPERTY(BlueprintAssignable, Category = DarkPower)
	FInTriggerTrapSeller InTriggerTrapsSellerChanged;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = DarkPower)
	bool isInTriggerTrapSeller;

	UFUNCTION(BlueprintCallable, Category = DarkPower)
	void SetIsInTriggerTrapSeller(bool value);

	UFUNCTION(BlueprintCallable)
	void LoseManaPoint(int value);
	void WinManaPoint(int value);

	UFUNCTION(BlueprintCallable)
	void RegenTrapBar();
	
	int GetManaPoints() const;

	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
};
