// Fill out your copyright notice in the Description page of Project Settings.


#include "Prop/ABGASFountain.h"
#include "GameFramework/RotatingMovementComponent.h"
#include "ArenaBattleGAS.h"
#include "AbilitySystemComponent.h"
#include "GameplayAbilitySpec.h"
#include "Tag/ABGameplayTag.h"
#include "Abilities/GameplayAbility.h"

AABGASFountain::AABGASFountain()
{
	ASC = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("ASC"));
	RotatingMovement = CreateDefaultSubobject<URotatingMovementComponent>(TEXT("RotateMovement"));
	ActionPeriod = 3.0f;
}

UAbilitySystemComponent* AABGASFountain::GetAbilitySystemComponent() const
{
	return ASC;
}

void AABGASFountain::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	RotatingMovement->bAutoActivate = false;
	RotatingMovement->Deactivate();

	// 오너 정보, 아바타 정보
	// 오너 정보 : 실제 ASC를 구동하고 데이터를 관리하는 실제 작업이 일어나고 있는 액터 정보
	// 아바타 정보 : 실제로 데이터를 처리하지 않지만 비주얼만 수행하는 액터를 지정
	// 아바타와 오너가 같으면 현재 이 분수대의 경우에는 데이터도 관리하고 실제 동작도 관리하는 형태로 작업을 초기화한다는 것
	ASC->InitAbilityActorInfo(this, this);

	for (const auto& StartAbility : StartAbilities)
	{
		FGameplayAbilitySpec StartSpec(StartAbility);
		ASC->GiveAbility(StartSpec);
	}
}

void AABGASFountain::BeginPlay()
{
	Super::BeginPlay();

	GetWorld()->GetTimerManager().SetTimer(ActionTimer, this, &AABGASFountain::TimerAction, ActionPeriod, true, 0.0f);
}

void AABGASFountain::TimerAction()
{
	ABGAS_LOG(LogABGAS, Log, TEXT("Begin"));
	
	FGameplayTagContainer TargetTag(ABTAG_ACTOR_ROTATE);

	if (!ASC->HasMatchingGameplayTag(ABTAG_ACTOR_ISROTATING))
	{
		ASC->TryActivateAbilitiesByTag(TargetTag);
	}
	else
	{
		ASC->CancelAbilities(&TargetTag);
	}
	//FGameplayAbilitySpec* RotateGASpec = ASC->FindAbilitySpecFromClass(UABGA_Rotate::StaticClass());
	//if (!RotateGASpec)
	//{
	//	ABGAS_LOG(LogABGAS, Error, TEXT("No Rotate Spec Found!"));
	//	return;
	//}

	//if (!RotateGASpec->IsActive())
	//{
	//	ASC->TryActivateAbility(RotateGASpec->Handle);
	//}
	//else
	//{
	//	ASC->CancelAbilityHandle(RotateGASpec->Handle);
	//}
	//if (!RotatingMovement->IsActive())
	//{
	//	RotatingMovement->Activate(true);
	//}
	//else
	//{
	//	RotatingMovement->Deactivate();
	//}
}
