// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "VehiclePC.generated.h"

/**
 * 
 */
UCLASS()
class VEHICLEBASE_API AVehiclePC : public APlayerController
{
	GENERATED_BODY()
	
public:

	/**
	* Handles attaching this controller to the specified pawn.
	* Only runs on the network authority (where HasAuthority() returns true).
	* @param InPawn The Pawn to be possessed.
	* @see HasAuthority()
	*/
	virtual void Possess(APawn* Pawn) override;

	/** Allows the PlayerController to set up custom input bindings. */
	virtual void SetupInputComponent() override;

protected:



private:
	
	/** called to move possessed pawn forward/backward */
	UFUNCTION()
	void MoveForward(float AxisValue);

	/** called to turn possessed pawn left/right */
	UFUNCTION()
	void MoveRight(float AxisValue);

	/** called to enable handle break */
	UFUNCTION()
	void HandbrakePressed();

	/** called to disable handle break */
	UFUNCTION()
	void HandbrakeReleased();

	// -----------------------------------------------------------------------------------

	/** possessed pawn vehicle interface reference  */
	class IVehicleInterface* VI;


};
