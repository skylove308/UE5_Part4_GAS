// Fill out your copyright notice in the Description page of Project Settings.


#include "GA/ABGA_Jump.h"
#include "GameFramework/Character.h"
#include "GA/AT/ABAT_JumpAndWaitForLanding.h"

UABGA_Jump::UABGA_Jump()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

// Ư�� �ɷ��� ���� �������� ���θ� �Ǵ��ϴ� �� ��� (���� ������ �������� ���� �˻�)
bool UABGA_Jump::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, OUT FGameplayTagContainer* OptionalRelevantTags) const
{
	// �θ� Ŭ������ CanActivateAbility�� ȣ���Ͽ�, �⺻����(��ٿ�, ��� ��)�� �����Ǵ��� Ȯ��
	bool bResult = Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);
	if (!bResult)
	{
		return false;
	}

	// ĳ���Ͱ� ���� �������� ���� ����(���� ����, ���� ���� ��)�� ������� �Ǵ�
	const ACharacter* Character = Cast<ACharacter>(ActorInfo->AvatarActor.Get());
	return (Character && Character->CanJump());
}

void UABGA_Jump::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	// �������Ʈ���� ����
	//UABAT_JumpAndWaitForLanding* JumpAndWaitingForLandingTask = UABAT_JumpAndWaitForLanding::CreateTask(this);
	//JumpAndWaitingForLandingTask->OnComplete.AddDynamic(this, &UABGA_Jump::OnLandedCallback);
	//JumpAndWaitingForLandingTask->ReadyForActivation();
}

// ����Ű�� ���� ������ ����
void UABGA_Jump::InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	ACharacter* Character = CastChecked<ACharacter>(ActorInfo->AvatarActor.Get());
	Character->StopJumping();
}

void UABGA_Jump::OnLandedCallback()
{
	// true: Ability ���ᰡ ������ Ŭ���̾�Ʈ �� ����ȭ ��, false: ���ÿ����� Ability�� ����
	bool bReplicatedEndAbility = true;
	// true: �ɷ��� ��ҵ� ���, false: �ɷ��� ���������� ����� ���
	bool bWasCanceled = false;
	// Ȱ��ȭ�� �ɷ� ����
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCanceled);
}
