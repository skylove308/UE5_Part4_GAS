// Fill out your copyright notice in the Description page of Project Settings.


#include "GA/ABGA_Rotate.h"
#include "GameFramework/RotatingMovementComponent.h"
#include "Tag/ABGameplayTag.h"

UABGA_Rotate::UABGA_Rotate()
{
	// AbilityTags는 해당 능력과 연관된 태그의 집합
	// 이 능력은 ABTAG_ACTOR_ROTATE 태그를 가집니다라는 것을 의미
	AbilityTags.AddTag(ABTAG_ACTOR_ROTATE);
	// ActivationOwnedTags는 능력이 활성화되었을 때 부여되는 태그를 정의
	// 이 태그는 능력이 활성화되었을 때 자동으로 부여되며, 다른 시스템이 이를 통해 현재 상태를 확인할 수 있음
	// 사용하는 예로, ABTAG_ACTOR_ISROTATING이 있는 동안 다른 회전 능력을 사용할 수 없도록 설정할 수 있음
	ActivationOwnedTags.AddTag(ABTAG_ACTOR_ISROTATING);
}

// 인자 설명
// 핸들 정보
// 액터 인포(오너 인포, 아바타 인포 등의 정보를 담은 구조체)
// 어떻게 발동을 했는지에 대한 정보를 담은 구조체
// 외부에서 어떻게 이것을 발동시켰는지에 대한 정보를 담은 이벤트 데이터라고 하는 구조체의 정보 
void UABGA_Rotate::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	// 아바타 액터가 보여지는 주체라서 가져오는 것이 일반적
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
