﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "ZombieSurvival/Public/Weapons/FinishReloadAnimNotify.h"

#include "ZombieSurvival/Public/Weapons/Gun.h"
#include "GameFramework/Actor.h"
#include "ZombieSurvival/Public/Characters/Player/ShooterCharacterInterface.h"

void UFinishReloadAnimNotify::Notify(
	USkeletalMeshComponent* MeshComp,
	UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference
)
{
	Super::Notify(MeshComp, Animation, EventReference);

	UE_LOG(LogTemp, Log, TEXT("Reload animation finished"));

	if (AActor* Owner = MeshComp->GetOwner())
	{
		UE_LOG(LogTemp, Log, TEXT("Reload animation finished 2"));

		if (Owner->GetClass()->ImplementsInterface(UShooterCharacterInterface::StaticClass()))
		{
			AGun* Gun = IShooterCharacterInterface::Execute_GetCurrentGun(Owner);
			if (Gun)
			{
				UE_LOG(LogTemp, Log, TEXT("Reload animation finished 3"));
				Gun->OnReloadAnimationFinished();
			}
		}
	}
}
