// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AIInterface.h"
#include "AIAgent.generated.h"

DECLARE_DELEGATE_TwoParams(FOnChaseFinished, class AAIAgentController*, class UBehaviorTreeComponent*);
DECLARE_DELEGATE_TwoParams(FOnAttackFinished, class AAIAgentController*, class UBehaviorTreeComponent*);

UCLASS()
class DEWARRIOR_API AAIAgent : public ACharacter, public IAIInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AAIAgent();

	virtual void ReceivedDamage(float damage) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere)
	class UWidgetComponent* healthWidgetComp;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> HealthWidgetClass;
public:	
	//BTT Relatedd
	FOnAttackFinished OnAttackFinished;
	FOnChaseFinished OnChaseFinished;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimMontage* attackMontage;

	virtual void Tick(float DeltaTime) override;

	//Chase Related
	void ChaseTarget();
	UFUNCTION()
	void FinishChase();

	//Attack Related
	UFUNCTION()
	void AttackTarget();
	UFUNCTION()
	void FinishAttack();

	//Rotation Relate
	void LookAtTarget(float DeltaTime);
	void LookAtMovementDirection(float DeltaTime);

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	FVector MovementDirection;

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	float Speed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health Relatedd")
	float initialHealth;
private:
	bool doChase = false;
	bool bCanReceiveDamage = true;
	FTimerHandle DamageCooldownTimer;

	AActor* targetActor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	float attackRange = 200.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float RotationSpeed = 5.0f;

	UPROPERTY()
	UAnimInstance* animInstance;

	bool IsTargetWithinAttackRange();

	void ResetDamageColdDown();

	UFUNCTION()
	void OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted);

};
