// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_BasicAttak.h"
#include "AIController.h"
#include "GameFramework/Pawn.h"
#include "AIAgent.h"
#include "AIAgentController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTT_BasicAttak::UBTT_BasicAttak()
{
	NodeName = "Attack Target";
}

EBTNodeResult::Type UBTT_BasicAttak::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	this->ownerCompPtr = &OwnerComp;
	AAIController* AIController = OwnerComp.GetAIOwner();
	this->controlledPawn = AIController ? AIController->GetPawn() : nullptr;

	if (this->controlledPawn)
	{
		UE_LOG(LogTemp, Warning, TEXT("I have controlled pawn(A): %s"), *this->controlledPawn->GetName());

		this->aiAgent = Cast<AAIAgent>(this->controlledPawn);
		if (this->aiAgent)
		{
			this->aiAgent->OnAttackFinished.Unbind();
			this->aiAgent->OnAttackFinished.BindUObject(this, &UBTT_BasicAttak::OnAttackFinished);
			this->aiAgent->AttackTarget();
			UE_LOG(LogTemp, Warning, TEXT("Executing Attack for AI: %s"), *this->controlledPawn->GetName());
			UE_LOG(LogTemp, Warning, TEXT("Task In Progress Attack!!!"));

			return EBTNodeResult::InProgress;
		}
	}
	UE_LOG(LogTemp, Warning, TEXT("Task Failed Attack!!!"));

	return EBTNodeResult::Failed;
}

void UBTT_BasicAttak::OnAttackFinished(AAIAgentController* controller, UBehaviorTreeComponent* bt)
{
	if (!controller || !bt)
	{
		UE_LOG(LogTemp, Error, TEXT("OnAttackFinished: AIController or OwnerComp is null!"));
		return;
	}

	APawn* ControlledPawn = controller->GetPawn();
	if (ControlledPawn)
	{
		UE_LOG(LogTemp, Warning, TEXT("Attack finished for AI: %s"), *ControlledPawn->GetName());

		AAIAgent* AIAgent = Cast<AAIAgent>(ControlledPawn);
		if (AIAgent)
		{
			UE_LOG(LogTemp, Warning, TEXT("Unbind the Event Attack!!!"));
			AIAgent->OnChaseFinished.Unbind();
		}

		FinishTask(bt);
	}
}

void UBTT_BasicAttak::FinishTask(UBehaviorTreeComponent* OwnerComp)
{
	FinishLatentTask(*OwnerComp, EBTNodeResult::Succeeded);
}
