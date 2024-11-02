// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterCharacter.h"

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
	GetComponents<USkeletalMeshComponent>(SkeletalComps);
	
	for (int i = 0; i < SkeletalComps.Num(); i++)
	{
		USkeletalMeshComponent* SkeletalComp = SkeletalComps[i];

		if (SkeletalComp->GetName() == "Gun")
		{
			GunMesh = SkeletalComp;
		}
	}
}

// Called every frame
void AShooterCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
	
	if (PlayerController)
	{
		FVector CameraLocation;
		FRotator CameraRotation;
		PlayerController->GetPlayerViewPoint(CameraLocation, CameraRotation);

		// Perform a line trace from the camera straight ahead
		FVector TraceStart = CameraLocation;
		FVector TraceEnd = TraceStart + (CameraRotation.Vector() * 10000.0f);
		FCollisionQueryParams CollisionParams;
		CollisionParams.AddIgnoredActor(this);

		GetWorld()->LineTraceSingleByChannel(CrosshairHitResult, TraceStart, TraceEnd, ECollisionChannel::ECC_Visibility, CollisionParams);
		// DrawDebugLine(GetWorld(), TraceStart, TraceEnd, CrosshairHitResult.bBlockingHit ? FColor::Green : FColor::Red, false, 0.1f, 0, 1.0f);

		if (CrosshairHitResult.bBlockingHit)
		{
			UE_LOG(LogTemp, Log, TEXT("Crosshair hit actor: %s"), *CrosshairHitResult.GetActor()->GetName());
		}
	}
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
		FHitResult HitResult = GetObjectInSight();

		if (LastHitResult.bBlockingHit && IsValid(LastHitResult.GetActor()))
		{
			UE_LOG(LogTemp, Log, TEXT("Trace hit actor: %s"), *LastHitResult.GetActor()->GetName());
			FVector ImpulseDirection = LastHitResult.ImpactPoint * 9999.0f;
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

FHitResult AShooterCharacter::GetObjectInSight()
{
	for (int i = 0; i < SkeletalComps.Num(); i++)
	{
		USkeletalMeshComponent* SkeletalComp = SkeletalComps[i];

		if (SkeletalComp->GetName() == "Gun")
		{
			GunMesh = SkeletalComp;
			FVector SocketLocation = SkeletalComp->GetSocketLocation("Muzzle");
			FRotator SocketRotation = SkeletalComp->GetSocketRotation("Muzzle");

			// add a trace to check if the projectile will hit something
			FVector TraceStart = SocketLocation;
			FVector TraceEnd = CrosshairHitResult.Location;
			FCollisionQueryParams CollisionParams;
			CollisionParams.AddIgnoredActor(this);

			GetWorld()->LineTraceSingleByChannel(LastHitResult, TraceStart, TraceEnd, ECollisionChannel::ECC_Visibility, CollisionParams);
			DrawDebugLine(GetWorld(), TraceStart, TraceEnd, LastHitResult.bBlockingHit ? FColor::Blue : FColor::Red, false, 5.0f, 0, 10.0f);
			UE_LOG(LogTemp, Log, TEXT("Tracing line: %s to %s"), *TraceStart.ToCompactString(), *TraceEnd.ToCompactString());
		}
	}
	return LastHitResult;
}