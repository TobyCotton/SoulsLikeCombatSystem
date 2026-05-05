// Fill out your copyright notice in the Description page of Project Settings.


#include "Animations/ToggleTraceNotifyState.h"
#include "Combat/TraceComponent.h"

void UToggleTraceNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	UTraceComponent* traceComp = MeshComp->GetOwner()->FindComponentByClass<UTraceComponent>();
	if (!IsValid(traceComp))
	{
		return;
	}
	traceComp->m_bIsAttacking = true;
}

void UToggleTraceNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	UTraceComponent* traceComp = MeshComp->GetOwner()->FindComponentByClass<UTraceComponent>();
	if (!IsValid(traceComp))
	{
		return;
	}
	traceComp->m_bIsAttacking = false;
}
