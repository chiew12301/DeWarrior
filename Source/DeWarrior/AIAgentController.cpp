// Fill out your copyright notice in the Description page of Project Settings.


#include "AIAgentController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "NavigationSystem.h"

AAIAgentController::AAIAgentController()
{
    this->BehaviorTreeComp = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorTreeComponent"));
    this->BlackboardComp = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComponent"));
}

void AAIAgentController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (InPawn)
	{
        this->mypawn = InPawn;
		FTimerHandle timerHandle;
		this->GetWorld()->GetTimerManager().SetTimer(timerHandle, this, &AAIAgentController::StartBehaviorTree, 0.2f, false);
	}
}

void AAIAgentController::StartBehaviorTree()
{
    this->myBehaviorTree = LoadObject<UBehaviorTree>(nullptr, TEXT("/Game/AI/BehaviorTree/BT_AI"));
    
    if (this->myBehaviorTree)
    {
        if (UseBlackboard(myBehaviorTree->BlackboardAsset, BlackboardComp))
        {
            if (RunBehaviorTree(this->myBehaviorTree))
            {
                ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
                if (PlayerCharacter)
                {
                    BlackboardComp->SetValueAsObject(FName(TEXT("AttackTarget")), PlayerCharacter);
                    UE_LOG(LogTemp, Warning, TEXT("Assigned Player to Attack Target for AI: %s"), *mypawn->GetName());
                }
            }
            else
            {
                UE_LOG(LogTemp, Warning, TEXT("Failed to run behavior tree for AI: %s"), *mypawn->GetName());
            }
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("Failed to initialize blackboard for AI: %s"), *mypawn->GetName());
        }
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("No Behavior Tree found for AI: %s"), *mypawn->GetName());
    }
}

void AAIAgentController::MoveToTarget(AActor* target)
{
	if (!target)
		return;

	MoveToActor(target, 5.0f);
}
