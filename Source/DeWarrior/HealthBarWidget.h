// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HealthBarWidget.generated.h"

/**
 * 
 */
UCLASS()
class DEWARRIOR_API UHealthBarWidget : public UUserWidget
{
	GENERATED_BODY()
protected:
	UPROPERTY(meta = (Bindwidget))
	class UProgressBar* healthBar;
	UPROPERTY(meta = (Bindwidget))
	class UTextBlock* currentHealthLabel;
public:
	void UpdateHealthBar(float health, float maxHealth);

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
};
