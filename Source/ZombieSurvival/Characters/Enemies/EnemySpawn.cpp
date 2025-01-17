#include "EnemySpawn.h"
#include "Enemy.h"
#include "ZombieSurvival/CharacterStatusComponent.h"

// Sets default values
AEnemySpawn::AEnemySpawn()
{
    // Set this actor to call Tick() every frame. You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AEnemySpawn::BeginPlay()
{
    Super::BeginPlay();
    GetWorldTimerManager().SetTimer(MemberTimerHandle, this, &AEnemySpawn::SpawnEnemies, SpawnDelay, true, 0.5f);
}

// Called every frame
// void AEnemySpawn::Tick(float DeltaTime)
// {
//     Super::Tick(DeltaTime);
// }

void AEnemySpawn::SpawnEnemies()
{
    if (bShouldSpawn)
    {
        UWorld* World = GetWorld();
        if (!World) return;

        // Get random location
        FVector SpawnLocation = GetActorLocation();
        FRotator SpawnRotation = GetActorRotation();
        FActorSpawnParameters SpawnParams;
        SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

        AEnemy* Enemy = World->SpawnActor<AEnemy>(EnemyBlueprint, SpawnLocation, SpawnRotation, SpawnParams);
        if (Enemy)
        {
            UCharacterStatusComponent* StatusComponent = Enemy->FindComponentByClass<UCharacterStatusComponent>();
            if (StatusComponent)
            {
                StatusComponent->MaxHealth += Wave * HealthMultiplier;
                StatusComponent->Heal(StatusComponent->MaxHealth);
            }

            EnemiesSpawned++;

            UE_LOG(LogTemp, Log, TEXT("Spawned enemy %d"), EnemiesSpawned);

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
    Wave++;
    EnemiesSpawned = 0;
    EnemiesToSpawn = FMath::Max(EnemiesToSpawn + Wave, 0);
    bShouldSpawn = true;
    GetWorldTimerManager().SetTimer(MemberTimerHandle, this, &AEnemySpawn::SpawnEnemies, SpawnDelay, true, 0.5f);
}