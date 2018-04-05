// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "VehicleInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UVehicleInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class VEHICLEBASE_API IVehicleInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	
	virtual void MoveForward(float AxisValue) = 0;
	virtual void MoveRight(float AxisValue) = 0;
	virtual void HandBreak(bool bPressed) = 0;
};
