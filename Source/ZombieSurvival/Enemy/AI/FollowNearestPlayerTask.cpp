// Fill out your copyright notice in the Description page of Project Settings.


#include "FollowNearestPlayerTask.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "ZombieSurvival/ShooterPlayer/ShooterCharacter.h"

UFollowNearestPlayerTask::UFollowNearestPlayerTask()
{
	DistanceThreshold = 1000.0f;
	TargetPlayer = nullptr;
}

EBTNodeResult::Type UFollowNearestPlayerTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	FVector NearestPlayerLocation = GetNearestPlayerLocation(OwnerComp);

	if (NearestPlayerLocation != FVector::ZeroVector)
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsVector("TargetLocation", NearestPlayerLocation);

		AAIController* AIController = OwnerComp.GetAIOwner();
		if (AIController)
		{
			AIController->MoveToLocation(NearestPlayerLocation);
			UE_LOG(LogTemp, Log, TEXT("Moving to location %s"), *NearestPlayerLocation.ToString());
		}
		
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
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
			UE_LOG(LogTemp, Log, TEXT("Moving to location %s"), *NearestPlayerLocation.ToString());

			if (Player)
			{
				TargetPlayer = Player;
				NearestPlayerLocation = Player->GetActorLocation();
				break;
			}
		}
	} else {
		float DistanceToPlayer = FVector::DistSquared(OwnerComp.GetAIOwner()->GetPawn()->GetActorLocation(), NearestPlayerLocation);

		if (DistanceToPlayer > 10000)
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