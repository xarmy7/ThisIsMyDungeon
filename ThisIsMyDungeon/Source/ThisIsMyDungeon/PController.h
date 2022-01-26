// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPossessed);

/**
 * 
 */
UCLASS()
class THISISMYDUNGEON_API APController : public APlayerController
{
	GENERATED_BODY()
	

private:

	//	Private Variable(s)
	//	--------------------

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Data, meta = (AllowPrivateAccess = "true"))
	class APlayableCharacter* playerCharacter;

protected:

public:

	//	Constructor(s) & Destructor(s)
	//	---------------------------------

	APController();

	//	Public Variable(s)
	//	-------------------

	UPROPERTY(BlueprintAssignable, Category = Controller)
	FOnPossessed OnPossessed;

	UPROPERTY(VisibleAnyWhere, BlueprintReadOnly, Category = "TrapsManager")
	class UTrapsLayingComponent* trapsLayingComponent;

	UPROPERTY(VisibleAnyWhere, BlueprintReadOnly, Category = "Grid")
	class UGridComponent* gridComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = DarkPower)
	int manaPoints = 800;

	//	Public Function(s)
	//	-------------------

	UFUNCTION(BlueprintCallable, Category = Data)
	class APlayableCharacter* GetPlayerCharacter();
	
	void BeginPlay();

	void OnPossess(APawn* inPawn) override;
};
