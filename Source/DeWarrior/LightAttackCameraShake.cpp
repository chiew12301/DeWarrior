// Fill out your copyright notice in the Description page of Project Settings.


#include "LightAttackCameraShake.h"

ULightAttackCameraShake::ULightAttackCameraShake()
{
    OscillationDuration = 0.05f;
    OscillationBlendInTime = 0.025f;
    OscillationBlendOutTime = 0.025f;

    RotOscillation.Pitch.Amplitude = FMath::RandRange(2.0f, 5.0f);
    RotOscillation.Pitch.Frequency = FMath::RandRange(10.0f, 20.0f);

    RotOscillation.Yaw.Amplitude = FMath::RandRange(2.0f, 5.0f);
    RotOscillation.Yaw.Frequency = FMath::RandRange(10.0f, 20.0f);
}