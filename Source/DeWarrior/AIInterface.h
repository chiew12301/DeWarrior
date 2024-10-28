// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "AIInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UAIInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class DEWARRIOR_API IAIInterface
{
	GENERATED_BODY()

public:
	virtual void ReceivedDamage(float damage);

	float maxHealth;
protected:
	float currentHealth;
};
