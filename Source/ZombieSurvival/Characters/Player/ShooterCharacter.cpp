// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterCharacter.h"

#include "InventoryComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AShooterCharacter::AShooterCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>(TEXT("InventoryComponent"));
}

// Called when the game starts or when spawned
void AShooterCharacter::BeginPlay()
{
	Super::BeginPlay();
	APlayerCameraManager* CameraManager = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0);

	CameraManager->ViewPitchMin = minAimOffset;
	CameraManager->ViewPitchMax = maxAimOffset;
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
