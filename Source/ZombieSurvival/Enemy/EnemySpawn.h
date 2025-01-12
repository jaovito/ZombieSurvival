// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemySpawn.generated.h"

UCLASS()
class ZOMBIESURVIVAL_API AEnemySpawn : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AEnemySpawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
UPROPERTY
	(EditAnywhere, BlueprintReadWrite, Category = "Enemy Spawn Settings")
	bool bShouldSpawn = true;
	int Wave = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy Spawn Settings")
	int EnemiesToSpawn = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy Spawn Settings")
	float SpawnDelay = 2.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy Spawn Settings")
	float HealthMultiplier = 1.0f;

	// enemy blueprint to spawn
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy Spawn Settings")
	TSubclassOf<class AEnemy> EnemyBlueprint;

	UFUNCTION(BlueprintCallable)
	void StartNextWave();

private:
	void SpawnEnemies();

	FTimerHandle MemberTimerHandle;

	int EnemiesSpawned = 0;
	
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
