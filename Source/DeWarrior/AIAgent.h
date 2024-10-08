// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AIAgent.generated.h"

UCLASS()
class DEWARRIOR_API AAIAgent : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AAIAgent();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void ChaseTarget();

	void AttackTarget();

private:
	AActor* targetActor;
	float attackRange = 200.0f;

	bool IsTargetWithinAttackRange();

};