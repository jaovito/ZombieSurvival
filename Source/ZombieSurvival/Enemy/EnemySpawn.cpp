// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemySpawn.h"

#include "Enemy.h"
#include "ZombieSurvival/CharacterStatusComponent.h"


// Sets default values
AEnemySpawn::AEnemySpawn()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AEnemySpawn::BeginPlay()
{
	Super::BeginPlay();
	GetWorldTimerManager().SetTimer(MemberTimerHandle, this, &AEnemySpawn::SpawnEnemies, SpawnDelay, true, 2.0f);
}

// Called every frame
void AEnemySpawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AEnemySpawn::SpawnEnemies()
{
	if (bShouldSpawn)
	{
		// get random location
		FVector SpawnLocation = GetActorLocation(); 
		FRotator SpawnRotation = GetActorRotation();
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

			
		AEnemy* Enemy = GetWorld()->SpawnActor<AEnemy>(EnemyBlueprint, SpawnLocation, SpawnRotation, SpawnParams);
		UCharacterStatusComponent* StatusComponent = Enemy->GetComponentByClass<UCharacterStatusComponent>();

		if (StatusComponent)
		{
			StatusComponent->MaxHealth += Wave * HealthMultiplier;
			StatusComponent->Heal(StatusComponent->MaxHealth);
		}
		
		EnemiesSpawned++;

		if (EnemiesSpawned >= EnemiesToSpawn)
		{
			bShouldSpawn = false;
			GetWorldTimerManager().ClearTimer(MemberTimerHandle);
		}
	}
}

void AEnemySpawn::StartNextWave()
{
	Wave++;
	EnemiesSpawned = 0;
	EnemiesToSpawn += Wave;
	bShouldSpawn = true;
	GetWorldTimerManager().SetTimer(MemberTimerHandle, this, &AEnemySpawn::SpawnEnemies, SpawnDelay, true, 2.0f);
}