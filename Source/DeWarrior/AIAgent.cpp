// Fill out your copyright notice in the Description page of Project Settings.


#include "AIAgent.h"
#include "AIAgentController.h"
#include "Kismet/GameplayStatics.h"
#include "NavigationSystem.h"
#include "AIController.h"
#include "HealthBarWidget.h"
#include <Components/WidgetComponent.h>

AAIAgent::AAIAgent()
{
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FClassFinder<UUserWidget> HealthWidgetBPClass(TEXT("/Game/Widget/WBP_HealthBar.WBP_HealthBar_C"));
	if (HealthWidgetBPClass.Succeeded())
	{
		HealthWidgetClass = HealthWidgetBPClass.Class;
	}

	this->healthWidgetComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthBarWidget"));
	this->healthWidgetComp->SetupAttachment(this->RootComponent);
	this->healthWidgetComp->SetWidgetSpace(EWidgetSpace::World);

	if (HealthWidgetClass)
	{
		this->healthWidgetComp->SetWidgetClass(HealthWidgetClass);
	}

	this->maxHealth = 5.0f;
	this->currentHealth = this->maxHealth;
}

void AAIAgent::ReceivedDamage(float damage)
{
	if (!this->bCanReceiveDamage) return;

	//UE_LOG(LogTemp, Warning, TEXT("Received Damage, Current Health: %s"), this->currentHealth);

	this->currentHealth -= damage;
	this->bCanReceiveDamage = false;
	UE_LOG(LogTemp, Warning, TEXT("Current Health: %f"), this->currentHealth);

	if (this->healthWidgetComp)
	{
		UHealthBarWidget* healthbarCast = Cast<UHealthBarWidget>(this->healthWidgetComp->GetUserWidgetObject());
		if (healthbarCast)
		{
			healthbarCast->UpdateHealthBar(this->currentHealth, this->maxHealth);
		}
	}


	if (this->currentHealth <= 0.0f)
	{
		this->Destroy();
		return;
	}

	GetWorld()->GetTimerManager().SetTimer(DamageCooldownTimer, this, &AAIAgent::ResetDamageColdDown, 2.0f, false);
}

void AAIAgent::BeginPlay()
{
	Super::BeginPlay();
	this->targetActor = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	this->animInstance = GetMesh()->GetAnimInstance();

	if (this->healthWidgetComp)
	{
		UHealthBarWidget* healthbarCast = Cast<UHealthBarWidget>(this->healthWidgetComp->GetUserWidgetObject());
		if (healthbarCast)
		{
			healthbarCast->UpdateHealthBar(this->currentHealth, this->maxHealth);
		}
	}
}

void AAIAgent::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (this->healthWidgetComp)
	{
		FVector CameraLocation = GetWorld()->GetFirstPlayerController()->PlayerCameraManager->GetCameraLocation();
		FRotator LookAtRotation = (CameraLocation - this->healthWidgetComp->GetComponentLocation()).Rotation();
		this->healthWidgetComp->SetWorldRotation(LookAtRotation);
	}

	if (!this->doChase)	return;

	if (!this->targetActor)
	{
		//UE_LOG(LogTemp, Warning, TEXT("Target is empty!"));
		this->FinishChase();
		return;
	}

	if (this->IsTargetWithinAttackRange())
	{
		//UE_LOG(LogTemp, Warning, TEXT("Reached target!"));
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
	//UE_LOG(LogTemp, Warning, TEXT("Chase START!"));
	this->doChase = true;
}

void AAIAgent::FinishChase()
{
	//UE_LOG(LogTemp, Warning, TEXT("Chase Finish!L %s"), *this->GetName());

	this->doChase = false;
	AAIAgentController* AIController = Cast<AAIAgentController>(GetController());
	this->OnChaseFinished.Execute(AIController, AIController->BehaviorTreeComp);
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
	//UE_LOG(LogTemp, Warning, TEXT("Finished Attack!"));
	AAIAgentController* AIController = Cast<AAIAgentController>(GetController());
	this->OnAttackFinished.Execute(AIController, AIController->BehaviorTreeComp);

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
	//UE_LOG(LogTemp, Warning, TEXT("Distance to target for AI %s: %f"), *GetName(), distanceToTarget);

	return distanceToTarget <= this->attackRange;
}

void AAIAgent::ResetDamageColdDown()
{
	this->bCanReceiveDamage = true;
}
