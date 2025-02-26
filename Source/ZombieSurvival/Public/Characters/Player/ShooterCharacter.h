// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InventoryComponent.h"
#include "ShooterCharacterInterface.h"
#include "GameFramework/Character.h"
#include "ZombieSurvival/CharacterStatusComponent.h"
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

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void AddItemToInventory_Implementation(AActor* Item) override;

	virtual AGun* GetCurrentGun_Implementation() override;

	virtual void Die_Implementation() override;

	virtual void TakeDamage_Implementation(float HitDamage) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shooting Settings")
	float minAimOffset = -55.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shooting Settings")
	float maxAimOffset = 55.0f;

	UFUNCTION(BlueprintPure)
	bool IsFirstBetweenValues(float Value, float Min, float Max);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Inventory")
	UInventoryComponent* InventoryComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="CharacterStatusComponent")
	UCharacterStatusComponent* CharacterStatusComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UUserWidget* DieScreenWidget;
};
