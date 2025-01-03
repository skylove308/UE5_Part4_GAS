// Fill out your copyright notice in the Description page of Project Settings.


#include "GA/ABGA_Attack.h"
#include "Character/ABCharacterBase.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "ArenaBattleGAS.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Character/ABComboActionData.h"

UABGA_Attack::UABGA_Attack()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

// UAbilityTask_PlayMontageAndWait를 통해 몽타주를 재생하고, 첫 번째 콤보 섹션을 시작
// StartComboTimer를 호출하여 콤보 입력 가능 시간을 설정
void UABGA_Attack::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	AABCharacterBase* ABCharacter = CastChecked<AABCharacterBase>(ActorInfo->AvatarActor.Get());
	CurrentComboData = ABCharacter->GetComboActionData();
	ABCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);

	UAbilityTask_PlayMontageAndWait* PlayAttackTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, TEXT("PlayAttack"), ABCharacter->GetComboActionMontage(), 1.0f, GetNextSection());
	PlayAttackTask->OnCompleted.AddDynamic(this, &UABGA_Attack::OnCompletedCallback);
	PlayAttackTask->OnInterrupted.AddDynamic(this, & UABGA_Attack::OnInterruptedCallback);
	PlayAttackTask->ReadyForActivation();

	StartComboTimer();
}

// 플레이어 입력이 발생하면 HasNextComboInput 플래그가 설정
void UABGA_Attack::InputPressed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	if (!ComboTimerHandle.IsValid())
	{
		HasNextComboInput = false;
	}
	else
	{
		HasNextComboInput = true;
	}
}

void UABGA_Attack::CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility)
{
	Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);
}

void UABGA_Attack::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	AABCharacterBase* ABCharacter = CastChecked<AABCharacterBase>(ActorInfo->AvatarActor.Get());
	ABCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);

	CurrentComboData = nullptr;
	CurrentCombo = 0;
	HasNextComboInput = false;
}

void UABGA_Attack::OnCompletedCallback()
{
	bool bReplicatedEndAbility = true;
	bool bWasCanceled = false;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCanceled);
}

void UABGA_Attack::OnInterruptedCallback()
{
	bool bReplicatedEndAbility = true;
	bool bWasCanceled = true;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCanceled);
}

// 다음 섹션 이름 계산
FName UABGA_Attack::GetNextSection()
{
	CurrentCombo = FMath::Clamp(CurrentCombo + 1, 1, CurrentComboData->MaxComboCount);
	FName NextSection = *FString::Printf(TEXT("%s%d"), *CurrentComboData->MontageSectionNamePrefix, CurrentCombo);
	return NextSection;
}

// 타이머 설정 및 콤보 입력 대기
void UABGA_Attack::StartComboTimer()
{
	int32 ComboIndex = CurrentCombo - 1;
	ensure(CurrentComboData->EffectiveFrameCount.IsValidIndex(ComboIndex));

	// 현재 콤보 단계에서 다음 입력을 받을 수 있는 프레임 수 / 애니메이션의 초당 프레임수(FPS)
	float ComboEffectiveTime = CurrentComboData->EffectiveFrameCount[ComboIndex] / CurrentComboData->FrameRate;
	if (ComboEffectiveTime > 0.0f)
	{
		GetWorld()->GetTimerManager().SetTimer(ComboTimerHandle, this, &UABGA_Attack::CheckComboInput, ComboEffectiveTime, false);
	}
}

// 타이머가 만료되면 CheckComboInput에서 입력 플래그를 확인
// 입력이 있다면 다음 섹션으로 이동(MontageJumpToSection)하고, 새로운 타이머를 설정하여 다음 콤보 입력을 기다림
// 입력이 없으면 onComplete 델리게이트가 호출되며 콜백함수 실행되고 종료
void UABGA_Attack::CheckComboInput()
{
	ComboTimerHandle.Invalidate();
	if (HasNextComboInput)
	{
		MontageJumpToSection(GetNextSection());
		StartComboTimer();
		HasNextComboInput = false;
	}
}
