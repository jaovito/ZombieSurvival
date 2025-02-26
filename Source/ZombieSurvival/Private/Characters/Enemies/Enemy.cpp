// Fill out your copyright notice in the Description page of Project Settings.


#include "ZombieSurvival/Public/Characters/Enemies/Enemy.h"

#include "AIController.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "ZombieSurvival/Public/Characters/Enemies/EnemySpawn.h"


// Sets default values
AEnemy::AEnemy()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	UCharacterMovementComponent* MoveComp = GetCharacterMovement();
	MoveComp->GravityScale = 2.0f;
	MoveComp->SetWalkableFloorAngle(75.0f);
	MoveComp->MaxStepHeight = 45.0f;
	MoveComp->bUseFlatBaseForFloorChecks = true;
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

void AEnemy::TakeDamage_Implementation(float HitDamage, FVector HitLocation, FVector ImpulseDirection)
{
	CharacterStatusComponent->TakeDamage(HitDamage);
	float CurrentHealth = CharacterStatusComponent->GetHealth();
	if (CurrentHealth <= 0)
	{
		Die();
		GetMesh()->AddImpulseAtLocation(ImpulseDirection * 5.0f, HitLocation);
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
			if (Enemy && Enemy->CharacterStatusComponent->GetHealth() > 0)
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

void AEnemy::Die()
{
	DetachFromControllerPendingDestroy();
	EnableRagdoll();
}

void AEnemy::EnableRagdoll()
{
	if (GetMesh())
	{
		if (AAIController* AIController = Cast<AAIController>(GetController()))
		{
			AIController->StopMovement();
			AIController->UnPossess();
		} 
		
		GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		GetMesh()->SetAllBodiesSimulatePhysics(true);
		GetMesh()->SetAllBodiesBelowSimulatePhysics(FName("pelvis"), true, true);
		GetCharacterMovement()->StopMovementImmediately();
		GetCharacterMovement()->DisableMovement();

		GetWorld()->GetTimerManager().SetTimer(
			DampingTimerHandle,
			this,
			&AEnemy::UpdateDampingAfterRagdoll,
			1.0f,
			false);

		// get capsule component
		if (UCapsuleComponent* CharCapsuleComponent = GetCapsuleComponent())
        {
            CharCapsuleComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
        }

		
		GetWorld()->GetTimerManager().SetTimer(
			RagdollTimerHandle,
			this,
			&AEnemy::DisableRagdollPhysics,
			RagdollDuration,
			false
		);
	}
}

void AEnemy::DisableRagdollPhysics()
{
	if (GetMesh())
	{
		GetWorld()->GetTimerManager().ClearTimer(RagdollTimerHandle);
		GetMesh()->PutAllRigidBodiesToSleep();
		GetMesh()->SetSimulatePhysics(false);
		GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		GetMesh()->SetAllBodiesSimulatePhysics(false);
		GetMesh()->SetPhysicsBlendWeight(PhysicsBlendWeight);
		GetMesh()->bNoSkeletonUpdate = true;
	}
}

void AEnemy::UpdateDampingAfterRagdoll()
{
	if (GetMesh())
	{
		GetMesh()->SetLinearDamping(200.0f);
		GetMesh()->SetAngularDamping(210.0f);
	}
}

