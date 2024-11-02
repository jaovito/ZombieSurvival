// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterCharacter.h"

#include "Projectile.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"

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
	APlayerCameraManager* CameraManager = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0);

	CameraManager->ViewPitchMin = minAimOffset;
	CameraManager->ViewPitchMax = maxAimOffset;
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
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && !IsShooting() && !IsReloading() && IsAiming())
	{
		bIsShooting = true;

		TArray<USkeletalMeshComponent*> SkeletalComps;
		GetComponents<USkeletalMeshComponent>(SkeletalComps);

		for (int i = 0; i < SkeletalComps.Num(); i++)
		{
			USkeletalMeshComponent* SkeletalComp = SkeletalComps[i];

			if (SkeletalComp->GetName() == "Gun")
			{
				FVector SocketLocation = SkeletalComp->GetSocketLocation("Muzzle");
				FRotator SocketRotation = SkeletalComp->GetSocketRotation("Muzzle");

				// add a trace to check if the projectile will hit something
				FHitResult HitResult;
				FVector TraceStart = SocketLocation;
				FVector TraceEnd = SocketLocation + SocketRotation.Vector() * 10000.0f;
				FCollisionQueryParams CollisionParams;
				CollisionParams.AddIgnoredActor(this);

				GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECollisionChannel::ECC_Pawn, CollisionParams);
				DrawDebugLine(GetWorld(), TraceStart, TraceEnd, HitResult.bBlockingHit ? FColor::Blue : FColor::Red, false, 5.0f, 0, 10.0f);
				UE_LOG(LogTemp, Log, TEXT("Tracing line: %s to %s"), *TraceStart.ToCompactString(), *TraceEnd.ToCompactString());

				if (HitResult.bBlockingHit && IsValid(HitResult.GetActor()))
				{
					UE_LOG(LogTemp, Log, TEXT("Trace hit actor: %s"), *HitResult.GetActor()->GetName());
					FVector ImpulseDirection = HitResult.ImpactPoint * 9999.0f;
					ImpulseDirection.Normalize();
					
					ImpulseDirection.Z += 10.0f;
					
					if (HitResult.GetComponent()->IsSimulatingPhysics())
					{
						HitResult.GetComponent()->AddRadialImpulse(HitResult.ImpactPoint, 100.f, 300.0f, ERadialImpulseFalloff::RIF_Constant, true);
					}
				}
				else {
					UE_LOG(LogTemp, Log, TEXT("No Actors were hit"));
				}
			}
		}
		
		AnimInstance->Montage_Play(ShootMontage);
	}
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

bool AShooterCharacter::IsFirstBetweenValues(float Value, float Min, float Max)
{
	return Value >= Min && Value <= Max;
}

void AShooterCharacter::ResetShooting_Implementation()
{
	bIsShooting = false;
}

void AShooterCharacter::OnMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if (Montage == ShootMontage)
	{
		bIsShooting = false;
	}
}