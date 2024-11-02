// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/ShooterCharacterInterface.h"
#include "ShooterCharacter.generated.h"

UCLASS()
class ZOMBIESURVIVAL_API AShooterCharacter : public ACharacter, public IShooterCharacterInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AShooterCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void OnMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shooting Settings")
	UBlueprint* BPProjectile;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	bool bIsShooting = false;
	bool bIsAiming = false;
	bool bIsReloading = false;
	TArray<USkeletalMeshComponent*> SkeletalComps;
	USkeletalMeshComponent* GunMesh;
	FHitResult LastHitResult;
	FHitResult CrosshairHitResult;

	FHitResult GetObjectInSight();

public:
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

	virtual void ResetShooting_Implementation() override;
};
