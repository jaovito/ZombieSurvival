// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CharacterStatusComponent.generated.h"


UCLASS(ClassGroup=(CharacterComponent), meta=(BlueprintSpawnableComponent))
class ZOMBIESURVIVAL_API UCharacterStatusComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UCharacterStatusComponent();
	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
							   FActorComponentTickFunction* ThisTickFunction) override;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	float Health = MaxHealth;
	float Stamina = 100.0f;

public:
	UFUNCTION(BlueprintCallable)
	void TakeDamage(float Damage);
	
	UFUNCTION(BlueprintCallable)
	void Heal(float Amount);

	UFUNCTION(BlueprintCallable)
	void RestoreStamina(float Amount);

	UFUNCTION(BlueprintCallable)
	void DrainStamina(float Amount);

	UFUNCTION(BlueprintPure)
	float GetHealth();

	UFUNCTION(BlueprintPure)
	float GetStamina();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Status")
	float MaxHealth = 100.0f;
};
