// Fill out your copyright notice in the Description page of Project Settings.


#include "ZombieSurvival/Public/Characters/Player/ShooterCharacter.h"

#include "Components/ProgressBar.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "ZombieSurvival/Public/Weapons/Gun.h"

// Sets default values
AShooterCharacter::AShooterCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>(TEXT("InventoryComponent"));
	CharacterStatusComponent = CreateDefaultSubobject<UCharacterStatusComponent>(TEXT("CharacterStatusComponent"));
}

// Called when the game starts or when spawned
void AShooterCharacter::BeginPlay()
{
	Super::BeginPlay();
	APlayerCameraManager* CameraManager = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0);

	CameraManager->ViewPitchMin = minAimOffset;
	CameraManager->ViewPitchMax = maxAimOffset;
	CharacterStatusComponent->Heal(CharacterStatusComponent->MaxHealth);
}

// Called every frame
void AShooterCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AShooterCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

bool AShooterCharacter::IsFirstBetweenValues(float Value, float Min, float Max)
{
	return Value >= Min && Value <= Max;
}

void AShooterCharacter::AddPlayerWidgetToInventory(UUserWidget* Widget)
{
	if (InventoryComponent)
    {
        InventoryComponent->InventoryWidget = Widget;
    }
}

void AShooterCharacter::AddItemToInventory_Implementation(AActor* Item)
{
	if (InventoryComponent)
	{
		UE_LOG(LogTemp, Log, TEXT("Adding item to inventory: %s"), *Item->GetName());
		InventoryComponent->AddItem(Item->GetFName(), Item);
	}
}

AGun* AShooterCharacter::GetCurrentGun_Implementation()
{
	// get first item
	TMap<FName, AActor*> Items = InventoryComponent->GetItems();
	if (Items.Num() > 0)
	{
		AActor* Item = Items.CreateConstIterator().Value();
		return Cast<AGun>(Item);
	}

	return nullptr;
}

void AShooterCharacter::TakeDamage_Implementation(float HitDamage)
{
	float Health = CharacterStatusComponent->GetHealth();
	UE_LOG(LogTemp, Log, TEXT("Player was hitted with %f damage"), HitDamage);
	UE_LOG(LogTemp, Log, TEXT("Player health %f"), Health);
	UProgressBar* HealthBar = Cast<UProgressBar>(InventoryComponent->InventoryWidget->GetWidgetFromName("HealthBar"));
	float CurrentHealth = Health - HitDamage;

	
	if (CurrentHealth <= 0)
	{
		HealthBar->SetPercent(0.0f);
		Execute_Die(this);
	} else
	{
		// set bar health from widget to 0
		if (HealthBar)
		{
			float MaxPlayerHealth = CharacterStatusComponent->MaxHealth;
			HealthBar->SetPercent(CurrentHealth / MaxPlayerHealth);
		}
		
		CharacterStatusComponent->TakeDamage(HitDamage);
	}
	
}

void AShooterCharacter::Die_Implementation()
{
	UE_LOG(LogTemp, Log, TEXT("Player died"));
	// enable ragdoll physics
	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	GetMesh()->SetCollisionProfileName(TEXT("Ragdoll"));

	GetCharacterMovement()->DisableMovement();
	// disable camera
	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (PlayerController)
	{
		PlayerController->DisableInput(PlayerController);
		PlayerController->SetIgnoreLookInput(true);
	}

	if (DieScreenWidget)
	{
		DieScreenWidget->AddToViewport();
	}
}