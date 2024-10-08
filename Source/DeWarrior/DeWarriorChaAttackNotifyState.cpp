// Fill out your copyright notice in the Description page of Project Settings.


#include "AIAgent.h"
#include "DeWarriorChaAttackNotifyState.h"
#include "Components/SkeletalMeshComponent.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"

void UDeWarriorChaAttackNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
    if (!MeshComp || !MeshComp->GetOwner()) return;

    AActor* Owner = MeshComp->GetOwner();

    FName SocketName = MeshSocketName;
    if (MeshComp->DoesSocketExist(SocketName))
    {
        FVector SocketLocation = MeshComp->GetSocketLocation(SocketName);
        FRotator SocketRotation = MeshComp->GetSocketRotation(SocketName);

        // Spawn a box collider at the socket location
        GeneratedBoxCollider = NewObject<UBoxComponent>(Owner);
        if (GeneratedBoxCollider)
        {
            GeneratedBoxCollider->RegisterComponent();
            GeneratedBoxCollider->SetWorldLocation(SocketLocation);
            GeneratedBoxCollider->SetWorldRotation(SocketRotation);
            GeneratedBoxCollider->SetBoxExtent(FVector(20.0f, 10.0f, 40.0f)); // Adjust the box size as per your needs
            GeneratedBoxCollider->AttachToComponent(MeshComp, FAttachmentTransformRules::KeepWorldTransform, SocketName);

            // Enable collision for the box
            GeneratedBoxCollider->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
            GeneratedBoxCollider->SetCollisionObjectType(ECollisionChannel::ECC_Pawn);
            GeneratedBoxCollider->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
            GeneratedBoxCollider->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
        }
    }
}

void UDeWarriorChaAttackNotifyState::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime)
{
    if (!MeshComp || !GeneratedBoxCollider) return;

    FName SocketName = MeshSocketName;
    FVector SocketLocation = MeshComp->GetSocketLocation(SocketName);
    FRotator SocketRotation = MeshComp->GetSocketRotation(SocketName);

    GeneratedBoxCollider->SetWorldLocation(SocketLocation);
    GeneratedBoxCollider->SetWorldRotation(SocketRotation);

    DrawDebugHitbox(MeshComp);
}

void UDeWarriorChaAttackNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
    if (GeneratedBoxCollider)
    {
        GeneratedBoxCollider->DestroyComponent();
    }
}

void UDeWarriorChaAttackNotifyState::OnHitboxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (OtherActor && OtherActor->IsA(AAIAgent::StaticClass()))
    {
        OtherActor->Destroy();
        UE_LOG(LogTemp, Warning, TEXT("AIAgent destroyed!"));
    }
}

void UDeWarriorChaAttackNotifyState::DrawDebugHitbox(USkeletalMeshComponent* MeshComp)
{
    if (!MeshComp || !GeneratedBoxCollider) return;

    FVector HitboxLocation = GeneratedBoxCollider->GetComponentLocation();
    FRotator HitboxRotation = GeneratedBoxCollider->GetComponentRotation();
    FVector HitboxExtent = GeneratedBoxCollider->GetScaledBoxExtent();

    // Draw debug box to visualize the hitbox
    DrawDebugBox(
        MeshComp->GetWorld(),
        HitboxLocation,
        HitboxExtent,
        HitboxRotation.Quaternion(),
        FColor::Green,
        false,
        -1.0f,
        0,
        5.0f
    );
}
