// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputAction.h"
#include "NiagaraSystem.h"
#include "Components/SphereComponent.h"
#include "InputAction.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
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
	float FireRate = 0.1f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Shooting Settings")
	int MaxAmmo = 30;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Shooting Settings")
	int CurrentAmmo = 30;
	
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

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Shooting Settings")
	UInputAction* ReloadAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> CrosshairWidgetClass;

	UPROPERTY()
	UUserWidget* CrosshairWidget;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> AmmoWidgetClass;

	UPROPERTY()
	UUserWidget* AmmoWidget;
	
	UFUNCTION(BlueprintCallable, Category = "Gun")
	void Pickup(ACharacter* Player);

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations")
	UAnimMontage* ShootMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations")
	UAnimMontage* ReloadMontage;
	
	UFUNCTION(BlueprintCallable, Category = "Gun")
	void Shoot();

	UFUNCTION(BlueprintCallable, Category = "Gun")
	void Reload();

	UFUNCTION(BlueprintCallable, Category = "Gun")
	bool SetAiming(bool bNewAiming);

	UFUNCTION(BlueprintPure, Category = "Gun")
	bool IsShooting();

	UFUNCTION(BlueprintPure, Category = "Gun")
	bool IsReloading();

	UFUNCTION(BlueprintPure, Category = "Gun")
	bool IsAiming();

	UFUNCTION(BlueprintPure, Category = "Gun")
	bool IsFirstBetweenValues(float Value, float Min, float Max);

	void OnReloadAnimationFinished();

private:
	bool bIsShooting = false;
	bool bIsAiming = false;
	bool bIsReloading = false;
	FTimerHandle FireRateTimerHandle;
	FTimerHandle ResetCameraTimerHandle;
	UTextBlock* AmmoTextBlock;

	void Aim(const FInputActionInstance& Instance);
	
	ACharacter* PlayerOwner;
	FHitResult CrosshairHitResult;
	FHitResult GetObjectInSight();

	void ResetShooting();
	void ResetCamera();
};
