#pragma once
#include "ZombieSurvival/Weapons/Gun.h"

#include "ShooterCharacterInterface.generated.h"

UINTERFACE(MinimalAPI, Blueprintable)
class UShooterCharacterInterface : public UInterface
{
	GENERATED_BODY()
};

class IShooterCharacterInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Shooter Character Interface")
	void TakeDamage(float HitDamage);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Shooter Character Interface")
	AGun* GetCurrentGun();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Shooter Character Interface")
	void AddItemToInventory(AActor* Item);
};
