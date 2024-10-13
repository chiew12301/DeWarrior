// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_BasicAttak.h"
#include "AIController.h"
#include "GameFramework/Pawn.h"
#include "AIAgent.h"
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
		this->aiAgent = Cast<AAIAgent>(this->controlledPawn);
		if (this->aiAgent)
		{
			this->aiAgent->OnAttackFinished.RemoveDynamic(this, &UBTT_BasicAttak::OnAttackFinished);
			this->aiAgent->OnAttackFinished.AddDynamic(this, &UBTT_BasicAttak::OnAttackFinished);
			this->aiAgent->AttackTarget();
			return EBTNodeResult::InProgress;
		}
	}

	return EBTNodeResult::Failed;
}

void UBTT_BasicAttak::OnAttackFinished()
{
	UE_LOG(LogTemp, Warning, TEXT("Finished Received!"));

	if (this->aiAgent)
	{
		this->aiAgent->OnAttackFinished.RemoveDynamic(this, &UBTT_BasicAttak::OnAttackFinished);
	}
	this->FinishTask(this->ownerCompPtr);
}

void UBTT_BasicAttak::FinishTask(UBehaviorTreeComponent* OwnerComp)
{
	FinishLatentTask(*OwnerComp, EBTNodeResult::Succeeded);
}
