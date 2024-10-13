// Fill out your copyright notice in the Description page of Project Settings.


#include "AIAgentController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "NavigationSystem.h"

void AAIAgentController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (InPawn)
	{
		FTimerHandle timerHandle;
		this->GetWorld()->GetTimerManager().SetTimer(timerHandle, this, &AAIAgentController::StartBehaviorTree, 0.2f, false);
	}
}

void AAIAgentController::StartBehaviorTree()
{
    UBehaviorTree* BehaviorTree = LoadObject<UBehaviorTree>(nullptr, TEXT("/Game/AI/BehaviorTree/BT_AI"));

    if (BehaviorTree)
    {
        RunBehaviorTree(BehaviorTree);

        UBlackboardComponent* BlackboardComp = GetBlackboardComponent();
        if (BlackboardComp)
        {
            ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
            if (PlayerCharacter)
            {
                UE_LOG(LogTemp, Warning, TEXT("Assigned Player To Attaack Target"));

                BlackboardComp->SetValueAsObject(FName(TEXT("AttackTarget")), PlayerCharacter);
            }
        }
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("No Behavior Tree found"));
    }
}

void AAIAgentController::MoveToTarget(AActor* target)
{
	if (!target)
		return;

	MoveToActor(target, 5.0f);
}
