// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "AIAgent.h"
#include "BTT_ChaseTarget.generated.h"

/**
 * 
 */
UCLASS()
class DEWARRIOR_API UBTT_ChaseTarget : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UBTT_ChaseTarget();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	UFUNCTION()
	void OnChaseFinished();
protected:
	void FinishTask(UBehaviorTreeComponent* OwnerComp);

	UPROPERTY()
	APawn* controlledPawn;

	UPROPERTY()
	AAIAgent* aiAgent;

	UBehaviorTreeComponent* ownerCompPtr;
};
