// Fill out your copyright notice in the Description page of Project Settings.


#include "AIAgentController.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "NavigationSystem.h"

void AAIAgentController::MoveToTarget(AActor* target)
{
	if (!target)
		return;

	MoveToActor(target, 5.0f);
}
