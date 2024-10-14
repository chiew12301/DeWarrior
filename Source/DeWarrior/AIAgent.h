// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AIAgent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAttackFinished);

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
	//BTT Relatedd
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnAttackFinished OnAttackFinished;
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnAttackFinished OnChaseFinished;

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

private:
	bool doChase = false;

	AActor* targetActor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	float attackRange = 200.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float RotationSpeed = 5.0f;

	UPROPERTY()
	UAnimInstance* animInstance;

	bool IsTargetWithinAttackRange();

	UFUNCTION()
	void OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted);

};
