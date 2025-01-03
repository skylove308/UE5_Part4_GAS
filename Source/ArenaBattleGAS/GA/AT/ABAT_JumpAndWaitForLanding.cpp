// Fill out your copyright notice in the Description page of Project Settings.


#include "GA/AT/ABAT_JumpAndWaitForLanding.h"
#include "GameFramework/Character.h"

UABAT_JumpAndWaitForLanding::UABAT_JumpAndWaitForLanding()
{
}

// UABAT_JumpAndWaitForLanding �۾� ���� �� ��ȯ
UABAT_JumpAndWaitForLanding* UABAT_JumpAndWaitForLanding::CreateTask(UGameplayAbility* OwningAbility)
{
	UABAT_JumpAndWaitForLanding* NewTask = NewAbilityTask<UABAT_JumpAndWaitForLanding>(OwningAbility);
	return NewTask;
}

// �۾� ����� ȣ��, ���� ���۰� ���� �̺�Ʈ ���ε� ����
void UABAT_JumpAndWaitForLanding::Activate()
{
	Super::Activate();

	ACharacter* Character = CastChecked<ACharacter>(GetAvatarActor());
	Character->LandedDelegate.AddDynamic(this, &UABAT_JumpAndWaitForLanding::OnLandedCallback);
	Character->Jump();

	// �۾��� �ƹ�Ÿ�� ���ۿ� �����ϰ� ������ ���
	SetWaitingOnAvatar();
}

// �۾��� �ı��ǰų� �ɷ��� ����� �� ȣ��
void UABAT_JumpAndWaitForLanding::OnDestroy(bool AbilityEnded)
{
	ACharacter* Character = CastChecked<ACharacter>(GetAvatarActor());
	Character->LandedDelegate.RemoveDynamic(this, &UABAT_JumpAndWaitForLanding::OnLandedCallback);

	Super::OnDestroy(AbilityEnded);
}

// ĳ���Ͱ� �������� �� ȣ��
void UABAT_JumpAndWaitForLanding::OnLandedCallback(const FHitResult& Hit)
{
	// �� �۾��� ������ Ȱ�� �������� Ȯ��
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		// OnComplete ��������Ʈ ȣ��
		OnComplete.Broadcast();
	}
}
