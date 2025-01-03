// Fill out your copyright notice in the Description page of Project Settings.


#include "GA/ABGA_Jump.h"
#include "GameFramework/Character.h"
#include "GA/AT/ABAT_JumpAndWaitForLanding.h"

UABGA_Jump::UABGA_Jump()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

// 특정 능력이 실행 가능한지 여부를 판단하는 데 사용 (점프 가능한 상태인지 조건 검사)
bool UABGA_Jump::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, OUT FGameplayTagContainer* OptionalRelevantTags) const
{
	// 부모 클래스의 CanActivateAbility를 호출하여, 기본조건(쿨다운, 비용 등)이 충족되는지 확인
	bool bResult = Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);
	if (!bResult)
	{
		return false;
	}

	// 캐릭터가 점프 가능한지 내부 상태(공중 여부, 착지 여부 등)를 기반으로 판단
	const ACharacter* Character = Cast<ACharacter>(ActorInfo->AvatarActor.Get());
	return (Character && Character->CanJump());
}

void UABGA_Jump::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	// 블루프린트에서 연결
	//UABAT_JumpAndWaitForLanding* JumpAndWaitingForLandingTask = UABAT_JumpAndWaitForLanding::CreateTask(this);
	//JumpAndWaitingForLandingTask->OnComplete.AddDynamic(this, &UABGA_Jump::OnLandedCallback);
	//JumpAndWaitingForLandingTask->ReadyForActivation();
}

// 점프키를 떼면 점프가 종료
void UABGA_Jump::InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	ACharacter* Character = CastChecked<ACharacter>(ActorInfo->AvatarActor.Get());
	Character->StopJumping();
}

void UABGA_Jump::OnLandedCallback()
{
	// true: Ability 종료가 서버와 클라이언트 간 동기화 됨, false: 로컬에서만 Ability를 종료
	bool bReplicatedEndAbility = true;
	// true: 능력이 취소된 경우, false: 능력이 정상적으로 종료된 경우
	bool bWasCanceled = false;
	// 활성화된 능력 종료
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCanceled);
}
