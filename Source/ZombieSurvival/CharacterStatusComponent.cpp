// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterStatusComponent.h"


UCharacterStatusComponent::UCharacterStatusComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UCharacterStatusComponent::BeginPlay()
{
	Super::BeginPlay();
}


// Called every frame
void UCharacterStatusComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                        FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}


void UCharacterStatusComponent::TakeDamage(float Damage)
{
	Health -= Damage;
	if (Health <= 0.0f)
	{
		Health = 0.0f;
		// Call a function to handle death
	}
}

void UCharacterStatusComponent::Heal(float Amount)
{
	Health += Amount;
	if (Health > 100.0f)
	{
		Health = 100.0f;
	}
}

void UCharacterStatusComponent::RestoreStamina(float Amount)
{
	Stamina += Amount;
	if (Stamina > 100.0f)
	{
		Stamina = 100.0f;
	}
}

void UCharacterStatusComponent::DrainStamina(float Amount)
{
	Stamina -= Amount;
	if (Stamina < 0.0f)
	{
		Stamina = 0.0f;
	}
}

float UCharacterStatusComponent::GetHealth()
{
	return Health;
}

float UCharacterStatusComponent::GetStamina()
{
	return Stamina;
}