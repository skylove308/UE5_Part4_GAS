// Fill out your copyright notice in the Description page of Project Settings.


#include "GA/TA/ABTA_Trace.h"
#include "Abilities//GameplayAbility.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"
#include "Physics/ABCollision.h"
#include "DrawDebugHelpers.h"

AABTA_Trace::AABTA_Trace()
{
}

// 타겟팅 시작 시 호출되며, 공격의 시작점(소스 액터)을 설정
void AABTA_Trace::StartTargeting(UGameplayAbility* Ability)
{
	Super::StartTargeting(Ability);

	// 능력을 소유한 캐릭터를 저장
	SourceActor = Ability->GetCurrentActorInfo()->AvatarActor.Get();
}

// 트레이스를 수행하고, 생성된 타겟 데이터를 델리게이트를 통해 호출자에게 전달
void AABTA_Trace::ConfirmTargetingAndContinue()
{
	if (SourceActor)
	{
		FGameplayAbilityTargetDataHandle DataHandle = MakeTargetData();
		TargetDataReadyDelegate.Broadcast(DataHandle);
	}
}

// 캐릭터를 기준으로 트레이스 작업을 수행하고, 타겟 데이터를 생성
FGameplayAbilityTargetDataHandle AABTA_Trace::MakeTargetData() const
{
	ACharacter* Character = CastChecked<ACharacter>(SourceActor);

	FHitResult OutHitResult;
	const float AttackRange = 100.0f;
	const float AttackRadius = 50.0f;

	FCollisionQueryParams Params(SCENE_QUERY_STAT(UABTA_Trace), false, Character);
	const FVector Forward = Character->GetActorForwardVector();
	const FVector Start = Character->GetActorLocation() + Forward * Character->GetCapsuleComponent()->GetScaledCapsuleRadius();
	const FVector End = Start + Forward * AttackRange;

	bool HitDetected = GetWorld()->SweepSingleByChannel(OutHitResult, Start, End, FQuat::Identity, CCHANNEL_ABACTION, FCollisionShape::MakeSphere(AttackRadius), Params);

	FGameplayAbilityTargetDataHandle DataHandle;
	if (HitDetected)
	{
		FGameplayAbilityTargetData_SingleTargetHit* TargetData = new FGameplayAbilityTargetData_SingleTargetHit(OutHitResult);
		DataHandle.Add(TargetData);
	}

#if ENABLE_DRAW_DEBUG
	if (bShowDebug)
	{
		FVector CapsuleOrigin = Start + (End - Start) * 0.5f;
		float CapsuleHalfHeight = AttackRange * 0.5f;
		FColor DrawColor = HitDetected ? FColor::Green : FColor::Red;
		DrawDebugCapsule(GetWorld(), CapsuleOrigin, CapsuleHalfHeight, AttackRadius, FRotationMatrix::MakeFromZ(Forward).ToQuat(), DrawColor, false, 5.0f);
	}
#endif

	return DataHandle;
}
