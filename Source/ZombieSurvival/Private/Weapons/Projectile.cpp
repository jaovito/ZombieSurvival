// Fill out your copyright notice in the Description page of Project Settings.

#include "ZombieSurvival/Public/Weapons/Projectile.h"

#include "ZombieSurvival/Public/Characters/Enemies/EnemyInterface.h"
#include "ZombieSurvival/Public/Characters/Player/ShooterCharacterInterface.h"


// Sets default values
AProjectile::AProjectile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ProjectileBoxCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("ProjectileBoxCollider"));
	SetRootComponent(ProjectileBoxCollider);

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	ProjectileMovementComponent->InitialSpeed = 5000.0f;
	ProjectileMovementComponent->MaxSpeed = 5000.0f;
	ProjectileMovementComponent->ProjectileGravityScale = 0.5f;
}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();

	ProjectileBoxCollider->OnComponentHit.AddDynamic(
		this,
		&AProjectile::OnComponentHit
	);
}

// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AProjectile::OnComponentHit(
	UPrimitiveComponent* HitComp, 
	AActor* OtherActor, 
	UPrimitiveComponent* OtherComp, 
	FVector NormalImpulse, 
	const FHitResult& Hit
)
{
	// Destroy the projectile
	UE_LOG(LogTemp, Log, TEXT("Projectile hit actor: %s"), *OtherActor->GetName());
	bool bIsEnemy = OtherActor->GetClass()->ImplementsInterface(UEnemyInterface::StaticClass());
	bool bIsPlayer = OtherActor->GetClass()->ImplementsInterface(UShooterCharacterInterface::StaticClass());

	if (bIsEnemy)
	{
		FVector ImpulseDirection = (Hit.ImpactPoint - GetActorLocation()).GetSafeNormal();
		IEnemyInterface::Execute_TakeDamage(OtherActor, ProjectileDamage, Hit.ImpactPoint, ImpulseDirection * 10000.0f);
	}

	if (!bIsPlayer)
	{
		Destroy();
	}
}
