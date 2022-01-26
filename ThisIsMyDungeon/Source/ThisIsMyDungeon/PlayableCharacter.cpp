// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayableCharacter.h"

#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "PController.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Engine/SkeletalMeshSocket.h"
#include "TrapsLayingComponent.h"
#include "GridComponent.h"
#include "DefenderGameMode.h"

#include "DrawDebugHelpers.h"

APlayableCharacter::APlayableCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	team = 1;

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	walkSpeed = GetCharacterMovement()->MaxWalkSpeed;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	FP_MuzzleLocation = CreateDefaultSubobject<USceneComponent>(TEXT("MuzzleLocation"));
	//FP_MuzzleLocation->SetupAttachment(FP_Gun);
	FP_MuzzleLocation->SetRelativeLocation(FVector(0.2f, 48.4f, -10.6f));

	time = 0.25f;
}



void APlayableCharacter::BeginPlay()
{
	Super::BeginPlay();

	GetWorldTimerManager().SetTimer(timerHandle, this, &APlayableCharacter::RepeatingFunction, repetitionDelay, true);

	ADefenderGameMode* GM = Cast<ADefenderGameMode>(GetWorld()->GetAuthGameMode());

	if (GM)
	{
		OnPlayerDeath.AddDynamic(GM, &ADefenderGameMode::PlayerDeath);
	}

	camRotRef = CameraBoom->GetRelativeRotation();
}



void APlayableCharacter::OnPossess(APController* newcontroller)
{
	playerController = newcontroller;
}



void APlayableCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAction("LMB", IE_Pressed, this, &APlayableCharacter::Charge);
	PlayerInputComponent->BindAction("LMB", IE_Released, this, &APlayableCharacter::Shoot);

	PlayerInputComponent->BindAction("Sprinting", IE_Pressed, this, &APlayableCharacter::Sprinting);
	PlayerInputComponent->BindAction("Sprinting", IE_Released, this, &APlayableCharacter::StopSprinting);

	PlayerInputComponent->BindAxis("MoveForward", this, &APlayableCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &APlayableCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &APlayableCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &APlayableCharacter::LookUpAtRate);
	
	if (playerController)
	{
		playerController->trapsLayingComponent->SetupTrapsManagerInput(PlayerInputComponent);
	}
}



void APlayableCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}



void APlayableCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}



void APlayableCharacter::MoveForward(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}



void APlayableCharacter::MoveRight(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}



void APlayableCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	rot = GetControlRotation();
	rot.Pitch = 0.f;

	SetActorRotation(rot);
}



void APlayableCharacter::RepeatingFunction()
{
	if (!canShoot) canShoot = true;

	mainTime += repetitionDelay;

	if (mainTime >= delay)
	{
		if (playerController)
		{
			playerController->manaPoints += addMaxManaPoint;
		}
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("ManaPoints: %d"), GetManaPoints()));
		mainTime = 0.f;
	}
}


void APlayableCharacter::Charge()
{
	if ((playerController && playerController->trapsLayingComponent->GetLaying()) || currentFireball != nullptr || charging == true) return;
	
	charging = true;
}



void APlayableCharacter::StopCharging()
{
	if(currentFireball) currentFireball->charging = false;
	currentFireball = nullptr;

	charging = false;
}



void APlayableCharacter::ChargeFireball()
{
	if (!GetMesh() || (playerController && playerController->trapsLayingComponent->GetLaying()) || currentFireball != nullptr || playerController == nullptr) return;

	if (fireballBaseClass != nullptr)
	{
		UWorld* const World = GetWorld();
		if (World != nullptr && canShoot)
		{
			const FRotator SpawnRotation = GetControlRotation();
			const FVector SpawnLocation = GetMesh()->GetSocketLocation("RightHandSocket");

			//Set Spawn Collision Handling Override
			FActorSpawnParameters ActorSpawnParams;
			ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

			// spawn the projectile at the muzzle
			currentFireball = World->SpawnActor<AFireball>(fireballBaseClass, SpawnLocation, SpawnRotation, ActorSpawnParams);

			if (currentFireball)
			{
				OnKill.AddDynamic(currentFireball, &AFireball::Explode);

				FAttachmentTransformRules rules = FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true);

				currentFireball->AttachToComponent(GetMesh(), rules, TEXT("RightHandSocket"));

				//	Register caster
				currentFireball->caster = this;

				currentFireball->charging = true;
			}
			canShoot = false;
		}
	}
}




void APlayableCharacter::Shoot()
{
	if (OnShoot.IsBound()) OnShoot.Broadcast();
}



void APlayableCharacter::LaunchFireball()
{
	if (currentFireball && GetFollowCamera() != nullptr)
	{
		FVector startPoint = GetFollowCamera()->GetComponentLocation();
		FVector forward = GetFollowCamera()->GetForwardVector();
		FVector targetLocation = startPoint + forward * 10000.f;

		FHitResult hit;

		if(GetWorld()->LineTraceSingleByChannel(hit, startPoint, targetLocation,ECollisionChannel::ECC_EngineTraceChannel1))
		{
			float distToHit = FVector::DistSquared(hit.Location, currentFireball->GetActorLocation());

			//	Check if different to self AND Check if is far enough to avoid irrealist throw
			if (hit.Actor != this && distToHit > 150.f * 150.f)
			{
				targetLocation = hit.Location;
			}
		}
		
		//	Set fieball direction
		FVector dir = targetLocation - currentFireball->GetActorLocation();
		dir.Normalize();

		//	Launch the fireball then detach it
		currentFireball->Launch(dir);
	}

	StopCharging();
}



void APlayableCharacter::Sprinting()
{
	GetCharacterMovement()->MaxWalkSpeed = 1000.f;
}



void APlayableCharacter::StopSprinting()
{
	GetCharacterMovement()->MaxWalkSpeed = walkSpeed;
}

void APlayableCharacter::WinManaPoint(int value)
{
	if(playerController)	playerController->manaPoints += value;

	if(GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, FString::Printf(TEXT("You win 10 points of Mana")));
	
	if (DarkPointChanged.IsBound()) DarkPointChanged.Broadcast(GetManaPoints());
}

void APlayableCharacter::LoseManaPoint(int value)
{
	if (playerController) playerController->manaPoints -= value;

	if (DarkPointChanged.IsBound()) DarkPointChanged.Broadcast(GetManaPoints());
}



void APlayableCharacter::Death()
{
	if (OnPlayerDeath.IsBound())
	{
		if (playerController) OnPlayerDeath.Broadcast(playerController->NetPlayerIndex);
	}
	if (playerController) playerController->bAutoManageActiveCameraTarget = false;

	Destroy();

}

void APlayableCharacter::SetIsInTriggerTrapSeller(bool value)
{
	isInTriggerTrapSeller = value;

	if (InTriggerTrapsSellerChanged.IsBound()) InTriggerTrapsSellerChanged.Broadcast(value);
}


int APlayableCharacter::GetManaPoints() const
{
	return playerController ? playerController->manaPoints : 0;
}

void APlayableCharacter::RegenTrapBar()
{
	if (playerController)
	{
		playerController->trapsLayingComponent->ReloadTrapBar(playerController->trapsLayingComponent->traps);
	}
}
