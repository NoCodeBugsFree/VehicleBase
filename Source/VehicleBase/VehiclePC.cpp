// Fill out your copyright notice in the Description page of Project Settings.

#include "VehiclePC.h"
#include "VehicleInterface.h"
#include "Vehicle.h"

void AVehiclePC::Possess(APawn* Pawn)
{
	Super::Possess(Pawn);

	/** setup vehicle interface  */
	if(IVehicleInterface* VITest = Cast<IVehicleInterface>(Pawn))
	{
		VI = VITest;
	}
}

void AVehiclePC::SetupInputComponent()
{
	Super::SetupInputComponent();

	/** handbrake  */
	InputComponent->BindAction("Handbrake", IE_Pressed, this, &AVehiclePC::HandbrakePressed);
	InputComponent->BindAction("Handbrake", IE_Released, this, &AVehiclePC::HandbrakeReleased);

	/** movement  */
	InputComponent->BindAxis("MoveForward", this, &AVehiclePC::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &AVehiclePC::MoveRight);
}

void AVehiclePC::MoveForward(float AxisValue)
{
	if (VI)
	{
		VI->MoveForward(AxisValue);
	}
		
}

void AVehiclePC::MoveRight(float AxisValue)
{
	if (VI)
	{
		VI->MoveRight(AxisValue);
	}
}

void AVehiclePC::HandbrakePressed()
{
	if (VI)
	{
		VI->HandBreak(true);
	}
}

void AVehiclePC::HandbrakeReleased()
{
	if (VI)
	{
		VI->HandBreak(false);
	}
}
