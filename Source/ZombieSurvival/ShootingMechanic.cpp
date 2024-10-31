#include "ShootingMechanic.h"

ShootingMechanic::ShootingMechanic()
	: bIsShooting(false), bIsReloading(false)
{
}

ShootingMechanic::~ShootingMechanic()
{
}

void ShootingMechanic::StartShooting()
{
	if (!bIsReloading)
	{
		bIsShooting = true;
		// Implement shooting logic here
	}
}

void ShootingMechanic::StopShooting()
{
	bIsShooting = false;
	// Implement logic to stop shooting here
}

void ShootingMechanic::Reload()
{
	bIsReloading = true;
	// Implement reloading logic here
	bIsReloading = false;
}

bool ShootingMechanic::SetAiming(bool bNewAiming)
{
	bIsAiming = bNewAiming;
	return bIsAiming;
}