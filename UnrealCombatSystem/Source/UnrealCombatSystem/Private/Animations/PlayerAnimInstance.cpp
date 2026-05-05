// Fill out your copyright notice in the Description page of Project Settings.


#include "Animations/PlayerAnimInstance.h"

void UPlayerAnimInstance::UpdateSpeed()
{
	APawn* pawn{ TryGetPawnOwner() };
	if (!IsValid(pawn))
	{
		return;
	}
	FVector currentVelocity{ pawn->GetVelocity() };
	m_currentSpeed = static_cast<float>(currentVelocity.Length());
}

void UPlayerAnimInstance::HandleUpdatedTarget(AActor* NewTargetActorRef)
{
	m_bIsInCombat = IsValid(NewTargetActorRef);
}

void UPlayerAnimInstance::UpdateDirection()
{
	APawn* PawnRef{ TryGetPawnOwner() };
	if (!IsValid(PawnRef))
	{
		return;
	}

	if (!m_bIsInCombat)
	{
		return;
	}

	m_CurrentDirection = CalculateDirection(PawnRef->GetVelocity(), PawnRef->GetActorRotation());
}
