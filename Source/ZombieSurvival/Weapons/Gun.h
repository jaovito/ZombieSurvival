// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputAction.h"
#include "NiagaraSystem.h"
#include "Components/SphereComponent.h"
#include "InputAction.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/Actor.h"
#include "Gun.generated.h"

UCLASS()
class ZOMBIESURVIVAL_API AGun : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AGun();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shooting Settings")
	float Damage = 10.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shooting Settings")
	UBlueprint* BPProjectile;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Shooting Settings")
	UNiagaraSystem* ShootImpactFX;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Shooting Settings")
	UNiagaraSystem* ShootMuzzleFX;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USkeletalMeshComponent* GunMesh;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USphereComponent* PickupArea;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Shooting Settings")
	UInputAction* ShootAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Shooting Settings")
	UInputAction* AimAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shooting Settings")
	float FireRate = 0.1f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> CrosshairWidgetClass;

	UPROPERTY()
	UUserWidget* CrosshairWidget;
	
	UFUNCTION(BlueprintCallable)
	void Pickup(ACharacter* Player);

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shooting Settings")
	float minAimOffset = -55.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shooting Settings")
	float maxAimOffset = 55.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shooting Settings")
	UAnimMontage* ShootMontage;
	
	UFUNCTION(BlueprintCallable)
	void Shoot();

	UFUNCTION(BlueprintCallable)
	void Reload();

	UFUNCTION(BlueprintCallable)
	bool SetAiming(bool bNewAiming);

	UFUNCTION(BlueprintPure)
	bool IsShooting();

	UFUNCTION(BlueprintPure)
	bool IsReloading();

	UFUNCTION(BlueprintPure)
	bool IsAiming();

	UFUNCTION(BlueprintPure)
	bool IsFirstBetweenValues(float Value, float Min, float Max);

private:
	bool bIsShooting = false;
	bool bIsAiming = false;
	bool bIsReloading = false;
	FTimerHandle FireRateTimerHandle;
	FTimerHandle ResetCameraTimerHandle;

	void Aim(const FInputActionInstance& Instance);
	
	ACharacter* PlayerOwner;
	FHitResult CrosshairHitResult;
	FHitResult GetObjectInSight();

	void ResetShooting();
	void ResetCamera();
};
