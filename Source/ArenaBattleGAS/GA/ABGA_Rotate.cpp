// Fill out your copyright notice in the Description page of Project Settings.


#include "GA/ABGA_Rotate.h"
#include "GameFramework/RotatingMovementComponent.h"
#include "Tag/ABGameplayTag.h"

UABGA_Rotate::UABGA_Rotate()
{
	// AbilityTags�� �ش� �ɷ°� ������ �±��� ����
	// �� �ɷ��� ABTAG_ACTOR_ROTATE �±׸� �����ϴٶ�� ���� �ǹ�
	AbilityTags.AddTag(ABTAG_ACTOR_ROTATE);
	// ActivationOwnedTags�� �ɷ��� Ȱ��ȭ�Ǿ��� �� �ο��Ǵ� �±׸� ����
	// �� �±״� �ɷ��� Ȱ��ȭ�Ǿ��� �� �ڵ����� �ο��Ǹ�, �ٸ� �ý����� �̸� ���� ���� ���¸� Ȯ���� �� ����
	// ����ϴ� ����, ABTAG_ACTOR_ISROTATING�� �ִ� ���� �ٸ� ȸ�� �ɷ��� ����� �� ������ ������ �� ����
	ActivationOwnedTags.AddTag(ABTAG_ACTOR_ISROTATING);
}

// ���� ����
// �ڵ� ����
// ���� ����(���� ����, �ƹ�Ÿ ���� ���� ������ ���� ����ü)
// ��� �ߵ��� �ߴ����� ���� ������ ���� ����ü
// �ܺο��� ��� �̰��� �ߵ����״����� ���� ������ ���� �̺�Ʈ �����Ͷ�� �ϴ� ����ü�� ���� 
void UABGA_Rotate::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	// �ƹ�Ÿ ���Ͱ� �������� ��ü�� �������� ���� �Ϲ���
	AActor* AvatarActor = ActorInfo->AvatarActor.Get();
	if (AvatarActor)
	{
		URotatingMovementComponent* RotatingMovement = Cast<URotatingMovementComponent>(AvatarActor->GetComponentByClass(URotatingMovementComponent::StaticClass()));
		if (RotatingMovement)
		{
			RotatingMovement->Activate(true);
		}
	}
}

void UABGA_Rotate::CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility)
{
	Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);

	AActor* AvatarActor = ActorInfo->AvatarActor.Get();
	if (AvatarActor)
	{
		URotatingMovementComponent* RotatingMovement = Cast<URotatingMovementComponent>(AvatarActor->GetComponentByClass(URotatingMovementComponent::StaticClass()));
		if (RotatingMovement)
		{
			RotatingMovement->Deactivate();
		}
	}
}