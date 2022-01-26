// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Button.h"
#include "TrapShopButton.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FClickWithIndex, const int, index);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPressedWithIndex, const int, index);

/**
 * 
 */
UCLASS()
class THISISMYDUNGEON_API UTrapShopButton : public UButton
{
	GENERATED_BODY()

	int trapIndex;

public :
	UTrapShopButton();

	UFUNCTION(BlueprintCallable, Category = Button)
	void SetTrapIndex(int value) { trapIndex = value; }

	UFUNCTION(BlueprintCallable, Category = Button)
	int GetTrapIndex() { return trapIndex; }

	UFUNCTION()
	void ExecuteOnClickedWithIndex();

	UFUNCTION()
	void ExecuteOnPressedWithIndex();

	UPROPERTY(BlueprintAssignable, Category = "Button|Event")
	FClickWithIndex OnClickedWithIndex;

	UPROPERTY(BlueprintAssignable, Category = "Button|Event")
	FPressedWithIndex OnPressedWithIndex;

protected :
};
