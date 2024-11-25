// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemySpawn.h"

#include "Enemy.h"
#include "Kismet/GameplayStatics.h"


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
	
}

// Called every frame
void AEnemySpawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TArray<AActor*> Enemies;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEnemy::StaticClass(), Enemies);

	if (Enemies.Num() == 0 && bShouldSpawn)
	{
		for (int i = 0; i < EnemiesToSpawn; i++)
		{
			// get random location
			FVector SpawnLocation = GetActorLocation(); 
			FRotator SpawnRotation = GetActorRotation();
			FActorSpawnParameters SpawnParams;
			SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

			AEnemy* Enemy = GetWorld()->SpawnActor<AEnemy>(AEnemy::StaticClass(), SpawnLocation, SpawnRotation, SpawnParams);
		}
	}
}

