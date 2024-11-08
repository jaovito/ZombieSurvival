// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "FollowNearestPlayerTask.generated.h"

/**
 * 
 */
UCLASS()
class ZOMBIESURVIVAL_API UFollowNearestPlayerTask : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UFollowNearestPlayerTask();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	FVector GetNearestPlayerLocation(UBehaviorTreeComponent& OwnerComp);

	AActor* TargetPlayer;
	
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	float DistanceThreshold;
};
