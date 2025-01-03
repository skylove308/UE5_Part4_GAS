// Fill out your copyright notice in the Description page of Project Settings.


#include "GA/AT/ABAT_Trace.h"
#include "GA/TA/ABTA_Trace.h"
#include "AbilitySystemComponent.h"

UABAT_Trace::UABAT_Trace()
{
}

UABAT_Trace* UABAT_Trace::CreateTask(UGameplayAbility* OwningAbility, TSubclassOf<class AABTA_Trace> TargetActorClass)
{
	UABAT_Trace* NewTask = NewAbilityTask<UABAT_Trace>(OwningAbility);
	NewTask->TargetActorClass = TargetActorClass;
	return NewTask;
}

void UABAT_Trace::Activate()
{
	Super::Activate();

	// 타겟 액터(AABTA_Trace)를 스폰하고 초기 설정을 수행
	SpawnAndInitializeTargetActor();

	// 타겟 액터의 스폰 작업을 완료하고, 타겟팅을 시작
	FinalizeTargetActor();

	// 이 작업이 아바타의 동작에 의존하고 있음을 명시
	SetWaitingOnAvatar();
}

void UABAT_Trace::OnDestroy(bool AbilityEnded)
{
	if (SpawnedTargetActor)
	{
		SpawnedTargetActor->Destroy();
	}

	Super::OnDestroy(AbilityEnded);
}

// 타겟 엑터(AABTA_Trace)를 스폰하고 초기 설정을 수행
void UABAT_Trace::SpawnAndInitializeTargetActor()
{
	// 타겟 액터를 스폰(생성)하되, 초기화 작업이 완료되지 않은 상태로 유지
	SpawnedTargetActor = Cast<AABTA_Trace>(Ability->GetWorld()->SpawnActorDeferred<AGameplayAbilityTargetActor>(TargetActorClass, FTransform::Identity, nullptr, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn));
	if (SpawnedTargetActor)
	{
		// 디버깅 정보를 표시
		SpawnedTargetActor->SetShowDebug(true);

		// 트레이스가 완료되었을 때 호출될 델리게이트를 바인딩
		SpawnedTargetActor->TargetDataReadyDelegate.AddUObject(this, &UABAT_Trace::OnTargetDataReadyCallback);
	}
}

// 스폰된 타겟 액터를 초기화하고 타겟팅을 시작
void UABAT_Trace::FinalizeTargetActor()
{
	UAbilitySystemComponent* ASC = AbilitySystemComponent.Get();
	if (ASC)
	{
		const FTransform SpawnTarnsform = ASC->GetAvatarActor()->GetTransform();
		// 타겟 액터의 스폰 작업을 완료하고, 트랜스폼(위치/회전)을 설정
		SpawnedTargetActor->FinishSpawning(SpawnTarnsform);

		// ASC에 새로 생성된 타겟 액터를 추가
		ASC->SpawnedTargetActors.Push(SpawnedTargetActor);

		// 타겟팅 작업을 시작하고, 즉시 완료
		SpawnedTargetActor->StartTargeting(Ability);
		SpawnedTargetActor->ConfirmTargeting();
	}
}

void UABAT_Trace::OnTargetDataReadyCallback(const FGameplayAbilityTargetDataHandle& DataHandle)
{
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		OnComplete.Broadcast(DataHandle);
	}

	EndTask();
}
