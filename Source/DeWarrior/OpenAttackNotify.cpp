// Fill out your copyright notice in the Description page of Project Settings.


#include "OpenAttackNotify.h"

void UOpenAttackNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	OnNotified.Broadcast();
	Super::Notify(MeshComp, Animation, EventReference);
}
