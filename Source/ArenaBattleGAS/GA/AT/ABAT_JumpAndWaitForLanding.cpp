// Fill out your copyright notice in the Description page of Project Settings.


#include "GA/AT/ABAT_JumpAndWaitForLanding.h"
#include "GameFramework/Character.h"

UABAT_JumpAndWaitForLanding::UABAT_JumpAndWaitForLanding()
{
}

// UABAT_JumpAndWaitForLanding 작업 생성 및 반환
UABAT_JumpAndWaitForLanding* UABAT_JumpAndWaitForLanding::CreateTask(UGameplayAbility* OwningAbility)
{
	UABAT_JumpAndWaitForLanding* NewTask = NewAbilityTask<UABAT_JumpAndWaitForLanding>(OwningAbility);
	return NewTask;
}

// 작업 실행시 호출, 점프 동작과 착지 이벤트 바인딩 설정
void UABAT_JumpAndWaitForLanding::Activate()
{
	Super::Activate();

	ACharacter* Character = CastChecked<ACharacter>(GetAvatarActor());
	Character->LandedDelegate.AddDynamic(this, &UABAT_JumpAndWaitForLanding::OnLandedCallback);
	Character->Jump();

	// 작업이 아바타의 동작에 의존하고 있음을 명시
	SetWaitingOnAvatar();
}

// 작업이 파괴되거나 능력이 종료될 때 호출
void UABAT_JumpAndWaitForLanding::OnDestroy(bool AbilityEnded)
{
	ACharacter* Character = CastChecked<ACharacter>(GetAvatarActor());
	Character->LandedDelegate.RemoveDynamic(this, &UABAT_JumpAndWaitForLanding::OnLandedCallback);

	Super::OnDestroy(AbilityEnded);
}

// 캐릭터가 착지했을 때 호출
void UABAT_JumpAndWaitForLanding::OnLandedCallback(const FHitResult& Hit)
{
	// 이 작업이 여전히 활성 상태인지 확인
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		// OnComplete 델리게이트 호출
		OnComplete.Broadcast();
	}
}
