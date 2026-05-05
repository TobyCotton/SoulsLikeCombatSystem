// Fill out your copyright notice in the Description page of Project Settings.


#include "Animations/LookAtPlayerNotifyState.h"
#include "Character/LookAtPlayerComponent.h"

void ULookAtPlayerNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	AActor* ownerRef{ MeshComp->GetOwner() };
	
	if (!IsValid(ownerRef))
	{
		return;
	}

	ULookAtPlayerComponent* rotationComp{ ownerRef->FindComponentByClass<ULookAtPlayerComponent>() };

	if (!IsValid(rotationComp))
	{
		return;
	}
	rotationComp->m_bCanRotate = true;
}

void ULookAtPlayerNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	AActor* ownerRef{ MeshComp->GetOwner() };

	if (!IsValid(ownerRef))
	{
		return;
	}

	ULookAtPlayerComponent* rotationComp{ ownerRef->FindComponentByClass<ULookAtPlayerComponent>() };

	if (!IsValid(rotationComp))
	{
		return;
	}

	rotationComp->m_bCanRotate = false;
}