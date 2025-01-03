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

	// Ÿ�� ����(AABTA_Trace)�� �����ϰ� �ʱ� ������ ����
	SpawnAndInitializeTargetActor();

	// Ÿ�� ������ ���� �۾��� �Ϸ��ϰ�, Ÿ������ ����
	FinalizeTargetActor();

	// �� �۾��� �ƹ�Ÿ�� ���ۿ� �����ϰ� ������ ���
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

// Ÿ�� ����(AABTA_Trace)�� �����ϰ� �ʱ� ������ ����
void UABAT_Trace::SpawnAndInitializeTargetActor()
{
	// Ÿ�� ���͸� ����(����)�ϵ�, �ʱ�ȭ �۾��� �Ϸ���� ���� ���·� ����
	SpawnedTargetActor = Cast<AABTA_Trace>(Ability->GetWorld()->SpawnActorDeferred<AGameplayAbilityTargetActor>(TargetActorClass, FTransform::Identity, nullptr, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn));
	if (SpawnedTargetActor)
	{
		// ����� ������ ǥ��
		SpawnedTargetActor->SetShowDebug(true);

		// Ʈ���̽��� �Ϸ�Ǿ��� �� ȣ��� ��������Ʈ�� ���ε�
		SpawnedTargetActor->TargetDataReadyDelegate.AddUObject(this, &UABAT_Trace::OnTargetDataReadyCallback);
	}
}

// ������ Ÿ�� ���͸� �ʱ�ȭ�ϰ� Ÿ������ ����
void UABAT_Trace::FinalizeTargetActor()
{
	UAbilitySystemComponent* ASC = AbilitySystemComponent.Get();
	if (ASC)
	{
		const FTransform SpawnTarnsform = ASC->GetAvatarActor()->GetTransform();
		// Ÿ�� ������ ���� �۾��� �Ϸ��ϰ�, Ʈ������(��ġ/ȸ��)�� ����
		SpawnedTargetActor->FinishSpawning(SpawnTarnsform);

		// ASC�� ���� ������ Ÿ�� ���͸� �߰�
		ASC->SpawnedTargetActors.Push(SpawnedTargetActor);

		// Ÿ���� �۾��� �����ϰ�, ��� �Ϸ�
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
