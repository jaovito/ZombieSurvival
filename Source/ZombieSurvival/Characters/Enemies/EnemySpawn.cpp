// EnemySpawn.cpp
#include "EnemySpawn.h"

#include "AIController.h"
#include "Enemy.h"
#include "Kismet/GameplayStatics.h"
#include "ZombieSurvival/CharacterStatusComponent.h"
#include "ZombieSurvival/Characters/Player/ShooterCharacter.h"

AEnemySpawn::AEnemySpawn()
{
    PrimaryActorTick.bCanEverTick = false;
}

void AEnemySpawn::BeginPlay()
{
    Super::BeginPlay();

    GetWorld()->GetTimerManager().SetTimer(FollowPlayerTimerHandle, this, &AEnemySpawn::MoveEnemiesToPlayer, 1.0f, true, 0.5f);

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

FVector AEnemySpawn::GetNearestPlayerLocation()
{
    TArray<AActor*> PlayerActors;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), AShooterCharacter::StaticClass(), PlayerActors);

    NearestPlayerLocation = FVector::ZeroVector;
    float MinDistance = TNumericLimits<float>::Max();

    for (AActor* Player : PlayerActors)
    {
        float Distance = FVector::DistSquared(GetActorLocation(), Player->GetActorLocation());
        if (Distance < MinDistance)
        {
            MinDistance = Distance;
            NearestPlayerLocation = Player->GetActorLocation();
        }
    }

    return NearestPlayerLocation;
}

void AEnemySpawn::MoveEnemiesToPlayer()
{
    NearestPlayerLocation = GetNearestPlayerLocation();
    if (NearestPlayerLocation != FVector::ZeroVector)
    {
        TArray<AActor*> EnemyActors;
        UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEnemy::StaticClass(), EnemyActors);

        for (AActor* EnemyActor : EnemyActors)
        {
            AEnemy* Enemy = Cast<AEnemy>(EnemyActor);
            if (Enemy && !Enemy->GetMesh()->IsSimulatingPhysics())
            {
                AAIController* AIController = Cast<AAIController>(Enemy->GetController());
                if (AIController)
                {
                    AIController->MoveToLocation(NearestPlayerLocation);
                }
            }
        }
    }
}
