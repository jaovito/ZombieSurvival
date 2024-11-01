// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterCharacter.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"

// Sets default values
AShooterCharacter::AShooterCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AShooterCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AShooterCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AShooterCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AShooterCharacter::Shoot()
{
	bIsShooting = true;
}

void AShooterCharacter::Reload()
{
	bIsReloading = true;
}

bool AShooterCharacter::IsReloading()
{
	return bIsReloading;
}

bool AShooterCharacter::IsShooting()
{
	return bIsShooting;
}

bool AShooterCharacter::IsAiming()
{
	return bIsAiming;
}

bool AShooterCharacter::SetAiming(bool bNewAiming)
{
	bIsAiming = bNewAiming;
	GetCharacterMovement()->bOrientRotationToMovement = !bNewAiming;
	USpringArmComponent* CameraBoom = FindComponentByClass<USpringArmComponent>();
	UCameraComponent* Camera = FindComponentByClass<UCameraComponent>();

	if (CameraBoom)
	{

		if (bNewAiming)
		{
			SetActorRotation(Camera->GetComponentRotation());
			
			constexpr float AimingBoomArmLength = 300.0f;
			CameraBoom->TargetArmLength = FMath::FInterpTo(CameraBoom->TargetArmLength, AimingBoomArmLength, GetWorld()->GetDeltaSeconds(), 10.0f);

			GetCharacterMovement()->MaxWalkSpeed = 250.0f;
		} else
		{
			GetCharacterMovement()->MaxWalkSpeed = 500.0f;
		}
	}

	return bNewAiming;
}