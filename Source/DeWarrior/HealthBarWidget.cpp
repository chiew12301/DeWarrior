// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthBarWidget.h"
#include <Components/ProgressBar.h>
#include <Components/TextBlock.h>
#include <GameFramework/PlayerController.h>
#include <Kismet/GameplayStatics.h>

void UHealthBarWidget::UpdateHealthBar(float health, float maxHealth)
{
	UE_LOG(LogTemp, Warning, TEXT("Called Update HB"));
	this->healthBar->SetPercent(health/maxHealth);

	this->currentHealthLabel->SetText(FText::AsNumber(health));
}

void UHealthBarWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
    Super::NativeTick(MyGeometry, InDeltaTime);
}