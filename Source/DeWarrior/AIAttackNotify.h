// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "Components/BoxComponent.h"
#include "AIAttackNotify.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, meta = (DisplayName = "AI Attack Notify State"))
class DEWARRIOR_API UAIAttackNotify : public UAnimNotifyState
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

    UFUNCTION()
    void OnHitboxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

    void DrawDebugHitbox(USkeletalMeshComponent* MeshComp);
};
