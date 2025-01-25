// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/EnemyInterface.h"
#include "ZombieSurvival/CharacterStatusComponent.h"
#include "Enemy.generated.h"

UCLASS()
class ZOMBIESURVIVAL_API AEnemy : public ACharacter, public IEnemyInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemy();

private:
	virtual void TakeDamage_Implementation(float HitDamage) override;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy Settings")
	bool bIsAttacking = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy Settings")
	bool bAlreadyAttacked = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Inventory")
	UCharacterStatusComponent* CharacterStatusComponent;
};
