#pragma once

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
};
