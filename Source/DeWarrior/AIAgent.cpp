// Fill out your copyright notice in the Description page of Project Settings.


#include "AIAgent.h"
#include "AIAgentController.h"
#include "Kismet/GameplayStatics.h"
#include "NavigationSystem.h"
#include "AIController.h"

// Sets default values
AAIAgent::AAIAgent()
{
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AAIAgent::BeginPlay()
{
	Super::BeginPlay();
	this->targetActor = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0); //This is only find the first player
}

// Called every frame
void AAIAgent::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!this->targetActor)
	{
		return;
	}

	if (this->IsTargetWithinAttackRange())
	{
		this->AttackTarget();
		return;
	}

	this->ChaseTarget();
	this->LookAtTarget(DeltaTime);

	Speed = GetVelocity().Size();

	// Determine movement direction
	if (Speed > 0)
	{
		MovementDirection = GetVelocity().GetSafeNormal();  // Get normalized movement direction
	}
	else
	{
		MovementDirection = FVector::ZeroVector;
	}
}

void AAIAgent::ChaseTarget()
{
	AAIAgentController* aiController = Cast<AAIAgentController>(GetController());
	if (aiController && this->targetActor)
	{
		aiController->MoveToTarget(this->targetActor);
	}
}

void AAIAgent::LookAtTarget(float DeltaTime)
{
	if (this->targetActor)
	{
		// Find the direction to the player
		FVector DirectionToPlayer = this->targetActor->GetActorLocation() - GetActorLocation();
		DirectionToPlayer.Z = 0; // Ignore vertical difference for rotation
		DirectionToPlayer.Normalize();

		// Calculate the desired rotation
		FRotator TargetRotation = FRotationMatrix::MakeFromX(DirectionToPlayer).Rotator();

		// Interpolate between the current and target rotation for smooth turning
		FRotator NewRotation = FMath::RInterpTo(GetActorRotation(), TargetRotation, DeltaTime, RotationSpeed);

		// Apply the new rotation
		SetActorRotation(NewRotation);
	}
}

void AAIAgent::AttackTarget()
{
	//UE_LOG(LogTemp, Warning, TEXT("Attacking Player!"));
}

bool AAIAgent::IsTargetWithinAttackRange()
{
	if(!this->targetActor)
		return false;

	float distanceToTarget = FVector::Dist(GetActorLocation(), this->targetActor->GetActorLocation());
	return distanceToTarget <= this->attackRange;
}
