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

	// ���� ����, �ƹ�Ÿ ����
	// ���� ���� : ���� ASC�� �����ϰ� �����͸� �����ϴ� ���� �۾��� �Ͼ�� �ִ� ���� ����
	// �ƹ�Ÿ ���� : ������ �����͸� ó������ ������ ���־� �����ϴ� ���͸� ����
	// �ƹ�Ÿ�� ���ʰ� ������ ���� �� �м����� ��쿡�� �����͵� �����ϰ� ���� ���۵� �����ϴ� ���·� �۾��� �ʱ�ȭ�Ѵٴ� ��
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
