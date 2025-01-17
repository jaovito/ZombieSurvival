#include "FollowNearestPlayerTask.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "ZombieSurvival/Characters/Enemies/Interfaces/EnemyInterface.h"
#include "ZombieSurvival/Characters/Player/ShooterCharacter.h"

UFollowNearestPlayerTask::UFollowNearestPlayerTask()
{
    DistanceThreshold = 1000.0f;
    DistanceToAttack = 30000.0f;
    TargetPlayer = nullptr;
}

EBTNodeResult::Type UFollowNearestPlayerTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    AAIController* AIController = OwnerComp.GetAIOwner();
    if (!AIController)
    {
        return EBTNodeResult::Failed;
    }

    ACharacter* Character = Cast<ACharacter>(AIController->GetPawn());
    if (!Character)
    {
        return EBTNodeResult::Failed;
    }

    FVector NearestPlayerLocation = GetNearestPlayerLocation(OwnerComp);
    if (NearestPlayerLocation != FVector::ZeroVector)
    {
        UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
        BlackboardComp->SetValueAsVector("TargetLocation", NearestPlayerLocation);

        bool bIsCharacterFalling = Character->GetCharacterMovement()->IsFalling();
        float DistanceToPlayer = FVector::DistSquared(Character->GetActorLocation(), NearestPlayerLocation);
        bool bIsCharacterInRange = DistanceToPlayer <= DistanceToAttack;

        if (bIsCharacterInRange)
        {
            AttackPlayer(OwnerComp);
        }

        if (!bIsCharacterFalling)
        {
            FollowPlayer(AIController, NearestPlayerLocation);
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
    }
}

void UFollowNearestPlayerTask::AttackPlayer(UBehaviorTreeComponent& OwnerComp)
{
    AShooterCharacter* Player = Cast<AShooterCharacter>(TargetPlayer);
    ACharacter* EnemyCharacter = Cast<ACharacter>(OwnerComp.GetAIOwner()->GetPawn());

    if (Player && EnemyCharacter)
    {
        bool bHasAttackImplementation = EnemyCharacter->GetClass()->ImplementsInterface(UEnemyInterface::StaticClass());

        if (bHasAttackImplementation)
        {
            IEnemyInterface::Execute_Attack(EnemyCharacter, Player);
        }
    }
}

FVector UFollowNearestPlayerTask::GetNearestPlayerLocation(UBehaviorTreeComponent& OwnerComp)
{
    TArray<AActor*> PlayerActors;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), AShooterCharacter::StaticClass(), PlayerActors);

    FVector NearestPlayerLocation = FVector::ZeroVector;
    float MinDistance = TNumericLimits<float>::Max();

    for (AActor* Player : PlayerActors)
    {
        float Distance = FVector::DistSquared(OwnerComp.GetAIOwner()->GetPawn()->GetActorLocation(), Player->GetActorLocation());
        if (Distance < MinDistance)
        {
            MinDistance = Distance;
            NearestPlayerLocation = Player->GetActorLocation();
            TargetPlayer = Player;
        }
    }

    return NearestPlayerLocation;
}