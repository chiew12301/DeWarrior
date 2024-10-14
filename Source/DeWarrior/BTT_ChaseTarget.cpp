// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_ChaseTarget.h"
#include "AIController.h"
#include "GameFramework/Pawn.h"
#include "AIAgent.h"
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

	if (this->controlledPawn)
	{
		this->aiAgent = Cast<AAIAgent>(this->controlledPawn);
		if (this->aiAgent)
		{
			this->aiAgent->OnChaseFinished.RemoveDynamic(this, &UBTT_ChaseTarget::OnChaseFinished);
			this->aiAgent->OnChaseFinished.AddDynamic(this, &UBTT_ChaseTarget::OnChaseFinished);
			this->aiAgent->ChaseTarget();
			return EBTNodeResult::InProgress;
		}
	}

	return EBTNodeResult::Failed;
}

void UBTT_ChaseTarget::OnChaseFinished()
{
	if (this->aiAgent)
	{
		this->aiAgent->OnChaseFinished.RemoveDynamic(this, &UBTT_ChaseTarget::OnChaseFinished);
	}
	this->FinishTask(this->ownerCompPtr);
}

void UBTT_ChaseTarget::FinishTask(UBehaviorTreeComponent* OwnerComp)
{
	FinishLatentTask(*OwnerComp, EBTNodeResult::Succeeded);
}
