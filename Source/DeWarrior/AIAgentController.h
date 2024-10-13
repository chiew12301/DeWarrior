// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "AIAgentController.generated.h"

UCLASS()
class DEWARRIOR_API AAIAgentController : public AAIController
{
	GENERATED_BODY()
	
public:
	virtual void OnPossess(APawn* InPawn) override;

	void StartBehaviorTree();

	void MoveToTarget(AActor* target);
};
