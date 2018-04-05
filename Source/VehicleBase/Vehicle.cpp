// Fill out your copyright notice in the Description page of Project Settings.

#include "Vehicle.h"
#include "WheeledVehicleMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/AudioComponent.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"
#include "ConstructorHelpers.h"
#include "VehicleBase.h"
#include "Kismet/GameplayStatics.h"
#include "WheeledVehicleMovementComponent.h"
#include "WheelBackBase.h"
#include "WheelFrontBase.h"

AVehicle::AVehicle()
{
	PrimaryActorTick.bCanEverTick = true;

	/** camera boom  */
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->SetRelativeLocation(FVector(0.f, 0.f, 250.f));
	CameraBoom->TargetArmLength = 1200.f;
	CameraBoom->SetRelativeRotation(FRotator(-10.f, 0.f, 0.f));
	
	/* camera component  */
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(CameraBoom);

	/** audio component  */
	EngineSound = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponent"));
	EngineSound->SetupAttachment(RootComponent);

#pragma region ConstructorHelpers

	/**  set the default audio */
	static ConstructorHelpers::FObjectFinder<USoundBase> SoundTemplate(TEXT("/Game/Sounds/CarDriveModel/Mono/EngineModel_Loop_Cue"));
	if (SoundTemplate.Succeeded())
	{
		EngineSound->SetSound(SoundTemplate.Object);
	}

	/** set the SandFX  */
	static ConstructorHelpers::FObjectFinder<UParticleSystem> SandFXParticleSystem(TEXT("/Game/Effects/Effects/VehicleEffects/P_Tarmac_wheel_kickup"));
	if (SandFXParticleSystem.Object)
	{
		SandFX = SandFXParticleSystem.Object;
	}

	/** set the TarmacFX  */
	static ConstructorHelpers::FObjectFinder<UParticleSystem> TarmacFXParticleSystem(TEXT("/Game/Effects/Effects/VehicleEffects/P_dirt_wheel_kickup"));
	if (TarmacFXParticleSystem.Object)
	{
		TarmacFX = TarmacFXParticleSystem.Object;
	}

	/** set the sound asset  */
	static ConstructorHelpers::FObjectFinder<USoundBase>StartEngineSoundTemplate(TEXT("/Game/Sounds/CarDriveModel/Mono/EngineIgnite_Cue"));
	if (StartEngineSoundTemplate.Object)
	{
		StartEngineSound = StartEngineSoundTemplate.Object;
	}


#pragma endregion

	// -----------------------------------------------------------------------------------
	//			setup Vehicle Movement comp
	// -----------------------------------------------------------------------------------

	/** setup wheel classes  */
	GetVehicleMovement()->WheelSetups[0].WheelClass = UWheelFrontBase::StaticClass();
	GetVehicleMovement()->WheelSetups[1].WheelClass = UWheelFrontBase::StaticClass();
	GetVehicleMovement()->WheelSetups[2].WheelClass = UWheelBackBase::StaticClass();
	GetVehicleMovement()->WheelSetups[3].WheelClass = UWheelBackBase::StaticClass();

	/** setup bone names  */
	GetVehicleMovement()->WheelSetups[0].BoneName = "F_L_wheelJNT";
	GetVehicleMovement()->WheelSetups[1].BoneName = "F_R_wheelJNT";
	GetVehicleMovement()->WheelSetups[2].BoneName = "B_L_wheelJNT";
	GetVehicleMovement()->WheelSetups[3].BoneName = "B_R_wheelJNT";
}

void AVehicle::MoveForward(float AxisValue)
{
	GetVehicleMovement()->SetThrottleInput(AxisValue);
}

void AVehicle::MoveRight(float AxisValue)
{
	GetVehicleMovement()->SetSteeringInput(AxisValue);
}

void AVehicle::HandBreak(bool bPressed)
{
	GetVehicleMovement()->SetHandbrakeInput(bPressed);
}

void AVehicle::BeginPlay()
{
	Super::BeginPlay();

	if (StartEngineSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, StartEngineSound, GetActorLocation());
	}
}

void AVehicle::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	/** called to set valid engine sound  */
	SetEngineSound();

	/** called to spawn surface-wheels interacting particles   */
	SpawnWheelsParticles();
}

void AVehicle::SpawnWheelsParticles()
{
	ForwardMovementSpeed = GetVehicleMovement()->GetForwardSpeed();
	if (ForwardMovementSpeed >= 10.f)
	{
		/** scale the particles according movement speed  */
		ParticlesScaleCoeff = FMath::Clamp(ForwardMovementSpeed / 3000.f, 0.1f, 1.f);

		for (FName WheelSocketName : WheelsSockets)
		{
			FVector Start = GetMesh()->GetSocketLocation(WheelSocketName);
			FVector End = Start;
			End.Z -= TraceDistance;

			FHitResult Hit;
			FCollisionQueryParams CollisionQueryParams;
			CollisionQueryParams.AddIgnoredActor(this);
			CollisionQueryParams.bReturnPhysicalMaterial = true;

			if (GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, CollisionQueryParams))
			{
				EPhysicalSurface PhysicalSurface = UPhysicalMaterial::DetermineSurfaceType(Hit.PhysMaterial.Get());

				UParticleSystem* SelectedFX = nullptr;
				switch (PhysicalSurface)
				{
				case SURFACE_SAND:	 SelectedFX = SandFX;	break;
				case SURFACE_TARMAC: SelectedFX = TarmacFX; break;
				default:									break;
				}

				if (SelectedFX)
				{
					UParticleSystemComponent* PSC = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), SelectedFX, Hit.ImpactPoint, FRotator::ZeroRotator/*Hit.ImpactNormal.Rotation()*/);
					if (PSC)
					{
						PSC->SetWorldScale3D(FVector(ParticlesScaleCoeff));
					}
				}
			}

		}
	}
}

void AVehicle::SetEngineSound()
{
	float InFloat = FMath::Abs(GetVehicleMovement()->GetEngineRotationSpeed());

	EngineSound->SetFloatParameter(RPM_Param, InFloat);
}
