// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Prop/ABFountain.h"
#include "ABGASFountain.generated.h"

/**
 * 
 */
UCLASS()
class ARENABATTLEGAS_API AABGASFountain : public AABFountain
{
	GENERATED_BODY()
	
public:
	AABGASFountain();

protected:
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;

	virtual void TimerAction();

protected:
	UPROPERTY(VisibleAnywhere, Category=Movement)
	TObjectPtr<class URotatingMovementComponent> RotatingMovement;

	UPROPERTY(EditAnywhere, Category=Timer)
	float ActionPeriod;

	FTimerHandle ActionTimer;
	
};
