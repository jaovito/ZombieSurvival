// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"

#include "EnemySpawn.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
AEnemy::AEnemy()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	CharacterStatusComponent = CreateDefaultSubobject<UCharacterStatusComponent>(TEXT("CharacterStatusComponent"));
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AEnemy::TakeDamage_Implementation(float HitDamage)
{
	CharacterStatusComponent->TakeDamage(HitDamage);
	float CurrentHealth = CharacterStatusComponent->GetHealth();
	if (CurrentHealth <= 0)
	{
		// enable ragdoll
		DetachFromControllerPendingDestroy();
		UCapsuleComponent* CapsuleComp = GetCapsuleComponent();
		CapsuleComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		CapsuleComp->SetCollisionResponseToAllChannels(ECR_Ignore);
		
		GetMesh()->SetCollisionProfileName(TEXT("Ragdoll"));
		SetActorEnableCollision(true);
		
		GetMesh()->SetAllBodiesSimulatePhysics(true);
		GetMesh()->SetSimulatePhysics(true);
		GetMesh()->WakeAllRigidBodies();
		GetMesh()->bBlendPhysics = true;
		SetActorTickEnabled(false);

		Destroy();

		if (this->GetCharacterMovement())
		{
			this->GetCharacterMovement()->StopMovementImmediately();
			this->GetCharacterMovement()->DisableMovement();
			this->GetCharacterMovement()->SetComponentTickEnabled(false);
		}
		
		// get all enemies
		TArray<AActor*> FoundActors;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEnemy::StaticClass(), FoundActors);

		int ActiveEnemies = 0;
		for (AActor* Actor : FoundActors)
		{
			AEnemy* Enemy = Cast<AEnemy>(Actor);
			if (Enemy && !Enemy->GetMesh()->IsSimulatingPhysics())
			{
				ActiveEnemies++;
			}
		}

		UE_LOG(LogTemp, Log, TEXT("Active enemies: %d"), ActiveEnemies);

		if (ActiveEnemies == 0)
		{
			// last enemy
			UE_LOG(LogTemp, Log, TEXT("Last enemy"));

			// get all enemy spawns class
			TArray<AActor*> EnemySpawns;
			UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEnemySpawn::StaticClass(), EnemySpawns);

			if (EnemySpawns.Num() > 0)
			{
				for (AActor* Actor : EnemySpawns)
				{
					AEnemySpawn* EnemySpawn = Cast<AEnemySpawn>(Actor);
					if (EnemySpawn)
					{
						EnemySpawn->StartNextWave();
					}
				}
			}
		}
	}
}