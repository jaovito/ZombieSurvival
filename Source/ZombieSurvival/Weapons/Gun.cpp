// Fill out your copyright notice in the Description page of Project Settings.


#include "Gun.h"

#include "NiagaraFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "Components/TextBlock.h"
#include "Components/TimelineComponent.h"
#include "ZombieSurvival/Characters/Player/Interfaces/PlayerAnimationInterface.h"
#include "Kismet/GameplayStatics.h"
#include "ZombieSurvival/Characters/Enemies/Interfaces/EnemyInterface.h"
#include "ZombieSurvival/Characters/Player/Interfaces/ShooterCharacterInterface.h"

// Sets default values
AGun::AGun()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// add skeletal mesh
	PickupArea = CreateDefaultSubobject<USphereComponent>(TEXT("PickupArea"));
	PickupArea->SetSphereRadius(100.0f);
	SetRootComponent(PickupArea);
	
	GunMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("GunMesh"));
	GunMesh->SetVisibility(true);
	GunMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GunMesh->SetupAttachment(PickupArea);

	CameraZoomTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("CameraZoomTimeline"));
}

// Called when the game starts or when spawned
void AGun ::BeginPlay()
{
	Super::BeginPlay();

	if (ZoomCurve)
	{
		InterpFunction.BindUFunction(this, FName("CameraZoomUpdate"));
		TimelineFinished.BindUFunction(this, FName("CameraZoomFinished"));

		CameraZoomTimeline->AddInterpFloat(ZoomCurve, InterpFunction, FName("Alpha"));
		CameraZoomTimeline->SetTimelineFinishedFunc(TimelineFinished);
	}
}

// Called every frame
void AGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AGun::Pickup(ACharacter* Player)
{
	// attach the gun to the player skeleton socket
	if (Player && !PlayerOwner)
	{
		PlayerOwner = Player;
		USkeletalMeshComponent* PlayerMesh = Player->GetMesh();
		if (PlayerMesh)
		{
			GunMesh->AttachToComponent(PlayerMesh, FAttachmentTransformRules::SnapToTargetIncludingScale, "Pistol");

			if (Player->GetClass()->ImplementsInterface(UShooterCharacterInterface::StaticClass()))
			{
				IShooterCharacterInterface::Execute_AddItemToInventory(Player, this);
			}
			
			// get the player input component
			if (GetWorld())
			{
				UEnhancedInputComponent* PlayerInputComponent = PlayerOwner->FindComponentByClass<UEnhancedInputComponent>();

				if (AmmoWidgetClass)
				{
					AmmoWidget = CreateWidget<UUserWidget>(GetWorld(), AmmoWidgetClass);
					if (AmmoWidget)
					{
						AmmoWidget->AddToViewport();
						// change text to display the current ammo
						AmmoTextBlock = Cast<UTextBlock>(AmmoWidget->GetWidgetFromName("Ammo"));
						if (AmmoTextBlock)
						{
							AmmoTextBlock->SetText(FText::Format(FText::FromString("{0}/{1}"), CurrentAmmo, MaxAmmo));
						}
					}
				}

				if (PlayerInputComponent)
				{
					if (ShootAction)
					{
						PlayerInputComponent->BindAction(ShootAction, ETriggerEvent::Triggered, this, &AGun::Shoot);
					}
					if (ReloadAction)
					{
						PlayerInputComponent->BindAction(ReloadAction, ETriggerEvent::Triggered, this, &AGun::Reload);
					}
					if (AimAction)
					{
						PlayerInputComponent->BindAction(AimAction, ETriggerEvent::Triggered, this, &AGun::Aim);
					}
				}
			}
		}
	}
}

void AGun::Shoot()
{
	if (GetWorld()->GetTimerManager().IsTimerActive(FireRateTimerHandle) || !PlayerOwner)
	{
		return;
	}

	if (CurrentAmmo <= 0)
	{
		return Reload();
	}

	UAnimInstance* AnimInstance = PlayerOwner->GetMesh()->GetAnimInstance();

	// log the if variables
	if (AnimInstance && !IsShooting() && !IsReloading() && IsAiming())
	{
		bIsShooting = true;
		FHitResult HitResult = GetObjectInSight();

		if (HitResult.bBlockingHit && IsValid(HitResult.GetActor()))
		{
			// UE_LOG(LogTemp, Log, TEXT("Trace hit actor: %s"), *HitResult.GetActor()->GetName());
			FVector ImpulseDirection = (HitResult.ImpactPoint - GetActorLocation()).GetSafeNormal();
			ImpulseDirection *= 10000.0f; // Adjust the impulse strength as needed
					
			ImpulseDirection.Z += 10.0f;
			bool bIsEnemy = HitResult.GetActor()->GetClass()->ImplementsInterface(UEnemyInterface::StaticClass());

			if (HitResult.GetComponent()->IsSimulatingPhysics())
			{
				HitResult.GetComponent()->AddImpulseAtLocation(ImpulseDirection, HitResult.ImpactPoint);
			}

			if (ShootImpactFX)
			{
				UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), ShootImpactFX, HitResult.ImpactPoint, FRotator::ZeroRotator, FVector(0.1f));
			}

			if (GunMesh && ShootMuzzleFX)
			{
				FVector MuzzleSocketLocation = GunMesh->GetSocketLocation("Muzzle");
				FRotator MuzzleSocketRotation = GunMesh->GetSocketRotation("Muzzle");

				UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), ShootMuzzleFX, MuzzleSocketLocation, MuzzleSocketRotation);
			}

			if (bIsEnemy)
			{
				IEnemyInterface::Execute_TakeDamage(HitResult.GetActor(), Damage);
			}
		}
		else {
			UE_LOG(LogTemp, Log, TEXT("No Actors were hit"));
		}

		AnimInstance->Montage_Play(ShootMontage);
		CurrentAmmo--;
		if (AmmoTextBlock)
		{
			AmmoTextBlock->SetText(FText::Format(FText::FromString("{0}/{1}"), CurrentAmmo, MaxAmmo));

			if (CurrentAmmo <= 0)
			{
				AmmoTextBlock->SetColorAndOpacity(FLinearColor::Red);
			}
		}
		
		GetWorld()->GetTimerManager().SetTimer(FireRateTimerHandle, this, &AGun::ResetShooting, FireRate, false);
	}
}

void AGun::Reload()
{
	if (IsReloading() || CurrentAmmo == MaxAmmo)
	{
		return;
	}
	
	bIsReloading = true;
	UAnimInstance* AnimInstance = PlayerOwner->GetMesh()->GetAnimInstance();

	if (AnimInstance && !IsShooting() && ReloadMontage)
	{
		AnimInstance->Montage_Play(ReloadMontage);
	}
}

void AGun::Aim(const FInputActionInstance& Instance)
{
	bool bNewAiming = Instance.GetValue().Get<bool>();

	if (bNewAiming != IsAiming())
	{
		SetAiming(bNewAiming);
	}
};

bool AGun::SetAiming(bool bNewAiming)
{
	if (!PlayerOwner)
	{
		return false;
	}
	
	bIsAiming = bNewAiming;
	PlayerOwner->GetCharacterMovement()->bOrientRotationToMovement = !bNewAiming;

		if (bNewAiming)
		{
			StartCameraZoom();
			PlayerOwner->GetCharacterMovement()->MaxWalkSpeed = 250.0f;

			if (CrosshairWidgetClass)
			{
				CrosshairWidget = CreateWidget<UUserWidget>(GetWorld(), CrosshairWidgetClass);
				if (CrosshairWidget)
				{
					CrosshairWidget->AddToViewport();
				}
			}
		} else
		{
			StopCameraZoom();
			PlayerOwner->GetCharacterMovement()->MaxWalkSpeed = 500.0f;

			if (CrosshairWidget)
			{
				CrosshairWidget->RemoveFromParent();
			}
		}

		IPlayerAnimationInterface::Execute_SetAiming(PlayerOwner->GetMesh()->GetAnimInstance(), bNewAiming);

	return bNewAiming;
}

bool AGun::IsShooting()
{
	return bIsShooting;
}

bool AGun::IsReloading()
{
	return bIsReloading;
}

bool AGun::IsAiming()
{
	return bIsAiming;
}

bool AGun::IsFirstBetweenValues(float Value, float Min, float Max)
{
	return Value >= Min && Value <= Max;
}

FHitResult AGun::GetObjectInSight()
{
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);

	if (PlayerController && IsAiming())
	{
		FVector CameraLocation;
		FRotator CameraRotation;
		PlayerController->GetPlayerViewPoint(CameraLocation, CameraRotation);

		FVector TraceStart = CameraLocation;
		FVector TraceEnd = TraceStart + (CameraRotation.Vector() * 10000.0f);
		FCollisionQueryParams CollisionParams;
		CollisionParams.AddIgnoredActor(this);

		GetWorld()->LineTraceSingleByChannel(CrosshairHitResult, TraceStart, TraceEnd, ECollisionChannel::ECC_Pawn, CollisionParams);
	}
	
	return CrosshairHitResult;
}

void AGun::ResetShooting()
{
	bIsShooting = false;
	GetWorld()->GetTimerManager().ClearTimer(FireRateTimerHandle);
}

void AGun::OnReloadAnimationFinished()
{
	bIsReloading = false;
	CurrentAmmo = MaxAmmo;
	
	if (AmmoTextBlock)
	{
		AmmoTextBlock->SetText(FText::Format(FText::FromString("{0}/{1}"), CurrentAmmo, MaxAmmo));
		AmmoTextBlock->SetColorAndOpacity(FLinearColor::White);
	}
}

void AGun::CameraZoomUpdate(float Value)
{
	UE_LOG(LogTemp, Log, TEXT("Camera zoom update"));
	if (PlayerOwner)
	{
		USpringArmComponent* CameraBoom = PlayerOwner->FindComponentByClass<USpringArmComponent>();
		if (CameraBoom)
		{
			float NewArmLength = FMath::Lerp(500.0f, 300.0f, Value);
			CameraBoom->TargetArmLength = NewArmLength;
		}
	}
}

void AGun::CameraZoomFinished()
{
	// Logic to execute when the zoom animation is finished
	UE_LOG(LogTemp, Log, TEXT("Camera zoom finished"));
}

void AGun::StartCameraZoom()
{
	if (CameraZoomTimeline)
	{
		CameraZoomTimeline->PlayFromStart();
		UE_LOG(LogTemp, Log, TEXT("Set aim"));
	}
}

void AGun::StopCameraZoom()
{
	if (CameraZoomTimeline)
	{
		CameraZoomTimeline->Reverse();
	}
}