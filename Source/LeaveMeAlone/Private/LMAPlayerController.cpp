// Fill out your copyright notice in the Description page of Project Settings.


#include "LMAPlayerController.h"

ALMAPlayerController::ALMAPlayerController()
{
}

void ALMAPlayerController::BeginPlay()
{
	Super::BeginPlay();

	SetInputMode(FInputModeGameOnly());
	bShowMouseCursor = false;
}