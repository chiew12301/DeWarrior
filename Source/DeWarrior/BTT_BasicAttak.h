// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "AIAgent.h"
#include "BTT_BasicAttak.generated.h"

/**
 * 
 */
UCLASS()
class DEWARRIOR_API UBTT_BasicAttak : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTT_BasicAttak();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	UFUNCTION()
	void OnAttackFinished();

protected:
	void FinishTask(UBehaviorTreeComponent* OwnerComp);
private:
	UPROPERTY()
	APawn* controlledPawn;

	UPROPERTY()
	AAIAgent* aiAgent;

	UBehaviorTreeComponent* ownerCompPtr;
};
