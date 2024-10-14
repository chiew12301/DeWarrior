// Fill out your copyright notice in the Description page of Project Settings.


#include "AIAgent.h"
#include "AIAgentController.h"
#include "Kismet/GameplayStatics.h"
#include "NavigationSystem.h"
#include "AIController.h"

AAIAgent::AAIAgent()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AAIAgent::BeginPlay()
{
	Super::BeginPlay();
	this->targetActor = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	this->animInstance = GetMesh()->GetAnimInstance();
}

void AAIAgent::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!this->doChase) return;

	if (!this->targetActor)
	{
		this->FinishChase();
		return;
	}

	if (this->IsTargetWithinAttackRange())
	{
		this->FinishChase();
		this->LookAtTarget(DeltaTime);
		return;
	}

	AAIAgentController* aiController = Cast<AAIAgentController>(GetController());
	if (aiController && this->targetActor)
	{
		aiController->MoveToTarget(this->targetActor);
	}

	this->LookAtMovementDirection(DeltaTime);
	FVector Velocity = GetVelocity();
	this->Speed = Velocity.Size();
}

void AAIAgent::ChaseTarget()
{
	this->doChase = true;
}

void AAIAgent::FinishChase()
{
	this->doChase = false;
	this->OnChaseFinished.Broadcast();
}

void AAIAgent::LookAtTarget(float DeltaTime)
{
	if (this->targetActor)
	{
		FVector DirectionToPlayer = this->targetActor->GetActorLocation() - GetActorLocation();
		DirectionToPlayer.Z = 0;
		DirectionToPlayer.Normalize();
		FRotator TargetRotation = FRotationMatrix::MakeFromX(DirectionToPlayer).Rotator();
		FRotator NewRotation = FMath::RInterpTo(GetActorRotation(), TargetRotation, DeltaTime, RotationSpeed);
		SetActorRotation(NewRotation);
	}
}

void AAIAgent::LookAtMovementDirection(float DeltaTime)
{
	FVector Velocity = GetVelocity();

	if (Velocity.SizeSquared() > 0.f)
	{
		FVector movdir = Velocity.GetSafeNormal();
		FRotator TargetRotation = movdir.Rotation();
		FRotator NewRotation = FMath::RInterpTo(GetActorRotation(), TargetRotation, DeltaTime, 5.0f);
		SetActorRotation(NewRotation);
	}
}

void AAIAgent::AttackTarget()
{
	if (!this->targetActor || !this->animInstance || !this->attackMontage)
	{
		return;
	}

	this->animInstance->Montage_Play(this->attackMontage);
	this->animInstance->OnMontageEnded.AddDynamic(this, &AAIAgent::OnAttackMontageEnded);
}

void AAIAgent::OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	this->FinishAttack();
}

void AAIAgent::FinishAttack()
{
	UE_LOG(LogTemp, Warning, TEXT("Finished Attack!"));
	OnAttackFinished.Broadcast();

	if (this->animInstance)
	{
		this->animInstance->OnMontageEnded.RemoveDynamic(this, &AAIAgent::OnAttackMontageEnded);
	}
}

bool AAIAgent::IsTargetWithinAttackRange()
{
	if(!this->targetActor)
		return false;

	float distanceToTarget = FVector::Dist(GetActorLocation(), this->targetActor->GetActorLocation());
	return distanceToTarget <= this->attackRange;
}
