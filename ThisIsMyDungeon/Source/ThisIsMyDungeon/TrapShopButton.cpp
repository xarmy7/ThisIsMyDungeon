// Fill out your copyright notice in the Description page of Project Settings.


#include "TrapShopButton.h"

UTrapShopButton::UTrapShopButton()
{
	FScriptDelegate scriptDelegate_click;
	scriptDelegate_click.BindUFunction(this, "ExecuteOnClickedWithIndex");

	OnClicked.Add(scriptDelegate_click);


	FScriptDelegate scriptDelegate_pressed;
	scriptDelegate_pressed.BindUFunction(this, "ExecuteOnPressedWithIndex");

	OnPressed.Add(scriptDelegate_pressed);
}

void UTrapShopButton::ExecuteOnClickedWithIndex()
{
	if (OnClickedWithIndex.IsBound())
		OnClickedWithIndex.Broadcast(trapIndex);
}

void UTrapShopButton::ExecuteOnPressedWithIndex()
{
	if (OnPressedWithIndex.IsBound())
		OnPressedWithIndex.Broadcast(trapIndex);
}