// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "AIController.h"
#include "AIAgentController.generated.h"

UCLASS()
class DEWARRIOR_API AAIAgentController : public AAIController
{
	GENERATED_BODY()
	
public:
	AAIAgentController();

	virtual void OnPossess(APawn* InPawn) override;

	void StartBehaviorTree();

	void MoveToTarget(AActor* target);

	UPROPERTY(BlueprintReadOnly, Category = "Debug Only")
	APawn* mypawn;

	UBehaviorTree* myBehaviorTree;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	UBehaviorTreeComponent* BehaviorTreeComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	UBlackboardComponent* BlackboardComp;
};
