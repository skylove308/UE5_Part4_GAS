// Fill out your copyright notice in the Description page of Project Settings.


#include "Prop/ABGASFountain.h"
#include "GameFramework/RotatingMovementComponent.h"
#include "ArenaBattleGAS.h"

AABGASFountain::AABGASFountain()
{
	RotatingMovement = CreateDefaultSubobject<URotatingMovementComponent>(TEXT("RotateMovement"));
	ActionPeriod = 3.0f;
}

void AABGASFountain::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	RotatingMovement->bAutoActivate = false;
	RotatingMovement->Deactivate();
}

void AABGASFountain::BeginPlay()
{
	Super::BeginPlay();

	GetWorld()->GetTimerManager().SetTimer(ActionTimer, this, &AABGASFountain::TimerAction, ActionPeriod, true, 0.0f);
}

void AABGASFountain::TimerAction()
{
	ABGAS_LOG(LogABGAS, Log, TEXT("Begin"));

	if (!RotatingMovement->IsActive())
	{
		RotatingMovement->Activate(true);
	}
	else
	{
		RotatingMovement->Deactivate();
	}
}
