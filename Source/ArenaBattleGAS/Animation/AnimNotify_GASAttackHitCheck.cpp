// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotify_GASAttackHitCheck.h"
#include "AbilitySystemBlueprintLibrary.h"

// UAnimNotify를 상속 받아 구현된 애니메이션 알림 클래스
// 애니메이션의 특정 프레임에서 호출되어 GAS와 연동하는 기능 제공
UAnimNotify_GASAttackHitCheck::UAnimNotify_GASAttackHitCheck()
{
}

FString UAnimNotify_GASAttackHitCheck::GetNotifyName_Implementation() const
{
	return TEXT("GASAttackHitCheck");
}

void UAnimNotify_GASAttackHitCheck::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (MeshComp)
	{
		AActor* OwnerActor = MeshComp->GetOwner();
		if (OwnerActor)
		{
			FGameplayEventData PayloadData;
			// 내가 지정한 어떤 특정한 액터에다가 태그를 넣어서 이벤트를 발동시킴
			UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(OwnerActor, TriggerGameplayTag, PayloadData);
		}
	}
}
