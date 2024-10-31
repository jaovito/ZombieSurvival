#pragma once

#include "CoreMinimal.h"

class ZOMBIESURVIVAL_API ShootingMechanic
{
public:
	ShootingMechanic();
	~ShootingMechanic();

	void StartShooting();
	void StopShooting();
	bool SetAiming(bool bNewAiming);
	void Reload();
	bool CanShoot() const { return !bIsReloading; }
	bool CanReload() const { return !bIsShooting; }
	bool IsAiming() const { return bIsAiming; }

private:
	bool bIsShooting;
	bool bIsReloading;
	bool bIsAiming;
};