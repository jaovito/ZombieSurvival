﻿// Fill out your copyright notice in the Description page of Project Settings.

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

	bool bShouldSpawn = true;
	int Wave = 0;
	int EnemiesToSpawn = 10;
	float SpawnDelay = 2.0f;
	float HealthMultiplier = 1.0f;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};