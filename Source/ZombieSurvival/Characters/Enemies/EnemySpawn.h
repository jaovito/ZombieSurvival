// EnemySpawn.h
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemySpawn.generated.h"

UCLASS()
class ZOMBIESURVIVAL_API AEnemySpawn : public AActor
{
	GENERATED_BODY()

public:
	AEnemySpawn();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy Spawn Settings")
	bool bShouldSpawn = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy Spawn Settings")
	int Wave = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy Spawn Settings")
	int EnemiesToSpawn = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy Spawn Settings")
	float SpawnDelay = 2.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy Spawn Settings")
	float HealthMultiplier = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy Spawn Settings")
	TSubclassOf<class AEnemy> EnemyBlueprint;

private:
	void SpawnEnemies();

	FTimerHandle MemberTimerHandle;
	FTimerHandle FollowPlayerTimerHandle;

	int EnemiesSpawned = 0;

	void MoveEnemiesToPlayer();
	FVector GetNearestPlayerLocation();

	FVector NearestPlayerLocation;

public:
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void StartNextWave();
};