// Fill out your copyright notice in the Description page of Project Settings.


#include "GA/ABGA_AttackHitCheck.h"
#include "ArenaBattleGAS.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "GA/AT/ABAT_Trace.h"
#include "GA/TA/ABTA_Trace.h"

UABGA_AttackHitCheck::UABGA_AttackHitCheck()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UABGA_AttackHitCheck::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	// 히트 체크를 수행하는 AT 생성
	UABAT_Trace* AttackTraceTask = UABAT_Trace::CreateTask(this, AABTA_Trace::StaticClass());

	AttackTraceTask->OnComplete.AddDynamic(this, &UABGA_AttackHitCheck::OnTraceResultCallback);
	AttackTraceTask->ReadyForActivation();
}

void UABGA_AttackHitCheck::OnTraceResultCallback(const FGameplayAbilityTargetDataHandle& TargetDataHandle)
{
	// UAbilitySystemBlueprintLibrary: 타겟 데이터에서 히트 결과를 추출하고 처리하기 위한 헬퍼 함수 제공
	// TargetDataHasResult : 주어진 타겟 데이터에 히트 결과가 포함되어 있는지 확인
	if (UAbilitySystemBlueprintLibrary::TargetDataHasHitResult(TargetDataHandle, 0))
	{
		// GetHitResultFromTargetData : 타겟 데이터에서 히트 결과 추출
		FHitResult HitResult = UAbilitySystemBlueprintLibrary::GetHitResultFromTargetData(TargetDataHandle, 0);
		ABGAS_LOG(LogABGAS, Log, TEXT("Target %s Detected"), *(HitResult.GetActor()->GetName()));
	}

	bool bReplicatedEndAbility = true;
	bool bWasCanceled = false;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCanceled);
}
