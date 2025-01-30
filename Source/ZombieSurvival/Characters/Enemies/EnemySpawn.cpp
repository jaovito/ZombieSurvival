// EnemySpawn.cpp
#include "EnemySpawn.h"
#include "Enemy.h"
#include "ZombieSurvival/CharacterStatusComponent.h"

AEnemySpawn::AEnemySpawn()
{
    PrimaryActorTick.bCanEverTick = false;
}

void AEnemySpawn::BeginPlay()
{
    Super::BeginPlay();

    if (bShouldSpawn)
    {
        StartNextWave();
    }
}

void AEnemySpawn::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void AEnemySpawn::SpawnEnemies()
{
    if (bShouldSpawn)
    {
        UWorld* World = GetWorld();
        if (!World) return;

        FVector SpawnLocation = GetActorLocation();
        FRotator SpawnRotation = GetActorRotation();
        FActorSpawnParameters SpawnParams;
        SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
        AEnemy* Enemy = World->SpawnActor<AEnemy>(EnemyBlueprint, SpawnLocation, SpawnRotation, SpawnParams);
        UE_LOG(LogTemp, Log, TEXT("Enemy spawned"));
        if (Enemy)
        {
            UE_LOG(LogTemp, Log, TEXT("Enemy is hidden: %d"), Enemy->IsHidden());
            
            UCharacterStatusComponent* StatusComponent = Enemy->FindComponentByClass<UCharacterStatusComponent>();
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
}

void AEnemySpawn::StartNextWave()
{
    if (EnemiesSpawned != 0 && EnemiesSpawned < EnemiesToSpawn)
    {
        return;
    }
    
    Wave++;
    EnemiesSpawned = 0;
    EnemiesToSpawn = FMath::Max(EnemiesToSpawn + Wave, 0);
    bShouldSpawn = true;
    GetWorldTimerManager().SetTimer(MemberTimerHandle, this, &AEnemySpawn::SpawnEnemies, SpawnDelay, true, 0.5f);
}