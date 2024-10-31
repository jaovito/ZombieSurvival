#pragma once

#include "CoreMinimal.h"

class ZOMBIESURVIVAL_API ShootingMechanic
{
public:
	ShootingMechanic();
	~ShootingMechanic();

	void StartShooting();
	void StopShooting();
	void Reload();
	bool CanShoot() const { return !bIsReloading; }
	bool CanReload() const { return !bIsShooting; }

private:
	bool bIsShooting;
	bool bIsReloading;
};