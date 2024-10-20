// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_ChaseTarget.h"
#include "AIController.h"
#include "GameFramework/Pawn.h"
#include "AIAgent.h"
#include "AIAgentController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTT_ChaseTarget::UBTT_ChaseTarget()
{
	NodeName = "Chase Target";
}

EBTNodeResult::Type UBTT_ChaseTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	this->ownerCompPtr = &OwnerComp;
	AAIController* AIController = OwnerComp.GetAIOwner();
	this->controlledPawn = AIController ? AIController->GetPawn() : nullptr;
	UE_LOG(LogTemp, Warning, TEXT("Controller Name(C): %s"), *AIController->GetName());

	if (this->controlledPawn)
	{
		UE_LOG(LogTemp, Warning, TEXT("I have controlled pawn(C): %s"), *this->controlledPawn->GetName());

		this->aiAgent = Cast<AAIAgent>(this->controlledPawn);
		if (this->aiAgent)
		{
			UE_LOG(LogTemp, Warning, TEXT("Ask Chase!"));
			this->aiAgent->OnChaseFinished.Unbind();
			this->aiAgent->OnChaseFinished.BindUObject(this, &UBTT_ChaseTarget::OnChaseFinished);
			this->aiAgent->ChaseTarget();
			UE_LOG(LogTemp, Warning, TEXT("Task In Progress Chase!!!"));

			return EBTNodeResult::InProgress;
		}
	}
	UE_LOG(LogTemp, Warning, TEXT("Task Failed Chase!!!"));

	return EBTNodeResult::Failed;
}

void UBTT_ChaseTarget::OnChaseFinished(AAIAgentController* controller, UBehaviorTreeComponent* bt)
{
	if (!controller || !bt)
	{
		UE_LOG(LogTemp, Error, TEXT("OnChaseFinished: AIController or OwnerComp is null!"));
		return;
	}

	APawn* ControlledPawn = controller->GetPawn();
	if (ControlledPawn)
	{
		UE_LOG(LogTemp, Warning, TEXT("Chase finished for AI: %s"), *ControlledPawn->GetName());

		AAIAgent* AIAgent = Cast<AAIAgent>(ControlledPawn);
		if (AIAgent)
		{
			UE_LOG(LogTemp, Warning, TEXT("Unbind the Event Chase!!!"));
			AIAgent->OnChaseFinished.Unbind();
		}

		FinishTask(bt);
	}
}

void UBTT_ChaseTarget::FinishTask(UBehaviorTreeComponent* OwnerComp)
{
	FinishLatentTask(*OwnerComp, EBTNodeResult::Succeeded);
}
