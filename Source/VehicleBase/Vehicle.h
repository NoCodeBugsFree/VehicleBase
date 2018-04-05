// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WheeledVehicle.h"
#include "VehicleInterface.h"
#include "Vehicle.generated.h"

/**
 * simple wheeled vehicle ABP setup
 * mesh space ref pose -> wheel handler ->
 * look at for each wheel (Bone to modify - F_L_Suspension --- Look at target (F_L_wheelJNT) look at axis of movement e.g. X-axis )
 * component to local -> final animation pose
 */
UCLASS()
class VEHICLEBASE_API AVehicle : public AWheeledVehicle, public IVehicleInterface
{
	GENERATED_BODY()
	
	/** camera boom  */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;
	
	/* camera component  */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* CameraComponent;
	
	/** audio component  */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Config", meta = (AllowPrivateAccess = "true"))
	class UAudioComponent* EngineSound;

	/* Default ImpactFX Emitter Template */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Config", meta = (AllowPrivateAccess = "true"))
	class UParticleSystem* SandFX;

	/* Flesh ImpactFX Emitter Template */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Config", meta = (AllowPrivateAccess = "true"))
	class UParticleSystem* TarmacFX;

	/** start engine sound */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Config", meta = (AllowPrivateAccess = "true"))
	class USoundBase* StartEngineSound;
	
public:

	AVehicle();

	//~ Begin Vehicle Interface 
	virtual void MoveForward(float AxisValue) override;
	virtual void MoveRight(float AxisValue) override;
	virtual void HandBreak(bool bPressed) override;
	//~ End Vehicle Interface 
	
protected:

	/** called when game starts or spawned  */
	virtual void BeginPlay() override;

	/** called to tick every frame  */
	virtual void Tick(float DeltaSeconds) override;
	
private:

	/** [Tick] called to set valid engine sound  */
	void SetEngineSound();

	/** [Tick] called to spawn surface-wheels interacting particles   */
	void SpawnWheelsParticles();

	/** line trace distance for particles spawning checking  */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Config", meta = (AllowPrivateAccess = "true"))
	float TraceDistance = 60.f;

	/** wheels sockets for line tracing  */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Config", meta = (AllowPrivateAccess = "true"))
	TArray<FName> WheelsSockets = { "FL_Socket", "FR_Socket", "BL_Socket", "BR_Socket" };

	/** RPM modifiable parameter to change engine sound level  */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Config", meta = (AllowPrivateAccess = "true"))
	FName RPM_Param = "RPM";

	float ParticlesScaleCoeff;
	float ForwardMovementSpeed;

public:

	

	
};
