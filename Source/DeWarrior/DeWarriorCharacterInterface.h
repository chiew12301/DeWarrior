// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "DeWarriorCharacterInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UDeWarriorCharacterInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class DEWARRIOR_API IDeWarriorCharacterInterface
{
	GENERATED_BODY()
public:
	virtual void ReceivedDamage(float damage);

protected:
	float currentHealth;
};
