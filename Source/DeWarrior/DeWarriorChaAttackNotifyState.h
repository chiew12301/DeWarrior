// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "Components/BoxComponent.h"
#include "DeWarriorChaAttackNotifyState.generated.h"

UCLASS(Blueprintable, meta = (DisplayName = "DeWarrior Character Notify State"))
class DEWARRIOR_API UDeWarriorChaAttackNotifyState : public UAnimNotifyState
{
	GENERATED_BODY()

public:
    virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration) override;

    virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime) override;

    virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hitbox")
    FName MeshSocketName;

    UPROPERTY(Transient)
    UBoxComponent* GeneratedBoxCollider;

    void DrawDebugHitbox(USkeletalMeshComponent* MeshComp);
};
