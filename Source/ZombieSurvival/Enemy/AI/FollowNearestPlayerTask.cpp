// Fill out your copyright notice in the Description page of Project Settings.


#include "FollowNearestPlayerTask.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "ZombieSurvival/CharacterStatusComponent.h"
#include "ZombieSurvival/ShooterPlayer/ShooterCharacter.h"

UFollowNearestPlayerTask::UFollowNearestPlayerTask()
{
	DistanceThreshold = 1000.0f;
	DistanceToAttack = 5500.0f;
	TargetPlayer = nullptr;
}

EBTNodeResult::Type UFollowNearestPlayerTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ACharacter* Character = Cast<ACharacter>(OwnerComp.GetAIOwner()->GetPawn());
	if (!Character)
	{
		return EBTNodeResult::Failed;
	}
	
	FVector NearestPlayerLocation = GetNearestPlayerLocation(OwnerComp);
	

	if (NearestPlayerLocation != FVector::ZeroVector)
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsVector("TargetLocation", NearestPlayerLocation);
		bool bIsCharacterFalling = Character->GetCharacterMovement()->IsFalling();
		float DistanceToPlayer = FVector::DistSquared(Character->GetActorLocation(), NearestPlayerLocation);
		bool bIsCharacterInRange = DistanceToPlayer < DistanceToAttack;

		UE_LOG(LogTemp, Log, TEXT("Character is in range: %d"), bIsCharacterInRange);
		UE_LOG(LogTemp, Log, TEXT("Character distance to player: %f"), DistanceToPlayer);
			UE_LOG(LogTemp, Log, TEXT("Character to attack: %f"), DistanceToAttack);
	
		if (bIsCharacterInRange)
		{
			AttackPlayer(OwnerComp);
		}
		
		if (!bIsCharacterFalling)
		{
			FollowPlayer(OwnerComp.GetAIOwner(), NearestPlayerLocation);
		}
		
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}

void UFollowNearestPlayerTask::FollowPlayer(AAIController* AIController, FVector NearestPlayerLocation)
{
	if (AIController)
	{
		AIController->MoveToLocation(NearestPlayerLocation);
		// UE_LOG(LogTemp, Log, TEXT("Moving to location %s"), *NearestPlayerLocation.ToString());
	}
}

void UFollowNearestPlayerTask::AttackPlayer(UBehaviorTreeComponent& OwnerComp)
{
	AShooterCharacter* Player = Cast<AShooterCharacter>(TargetPlayer);

	if (Player)
	{
		// get CharacterStatus component from player
		UCharacterStatusComponent* CharacterStatusComponent = Player->FindComponentByClass<UCharacterStatusComponent>();
		CharacterStatusComponent->TakeDamage(10.0f);
	}
}

FVector UFollowNearestPlayerTask::GetNearestPlayerLocation(UBehaviorTreeComponent& OwnerComp)
{
	TArray<AActor*> PlayerActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AShooterCharacter::StaticClass(), PlayerActors);

	FVector NearestPlayerLocation = FVector::ZeroVector;

	if (!TargetPlayer)
	{
		for (AActor* PlayerActor : PlayerActors)
		{
			AShooterCharacter* Player = Cast<AShooterCharacter>(PlayerActor);
			// UE_LOG(LogTemp, Log, TEXT("Moving to location %s"), *NearestPlayerLocation.ToString());

			if (Player)
			{
				TargetPlayer = Player;
				NearestPlayerLocation = Player->GetActorLocation();
				break;
			}
		}
	} else {
		float DistanceToPlayer = FVector::DistSquared(OwnerComp.GetAIOwner()->GetPawn()->GetActorLocation(), NearestPlayerLocation);

		if (DistanceToPlayer > DistanceThreshold)
		{
			TargetPlayer = nullptr;
			NearestPlayerLocation = GetNearestPlayerLocation(OwnerComp);
		} else
		{
			NearestPlayerLocation = TargetPlayer->GetActorLocation();
		}
	}

	return NearestPlayerLocation;
}