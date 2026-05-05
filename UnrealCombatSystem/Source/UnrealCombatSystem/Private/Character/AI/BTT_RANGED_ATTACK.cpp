// Fill out your copyright notice in the Description page of Project Settings.


#include "AIController.h"
#include "GameFramework/Character.h"
#include "Interfaces/Fighter.h"
#include "Character/AI/BTT_RANGED_ATTACK.h"
#include "Kismet/KismetMathLibrary.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Character/eEnemystate.h"

EBTNodeResult::Type UBTT_RANGED_ATTACK::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ACharacter* CharacterRef{OwnerComp.GetAIOwner()->GetPawn<ACharacter>()};

	if (!IsValid(CharacterRef))
	{ 
		return EBTNodeResult::Failed; 
	}

	float Distance{ OwnerComp.GetBlackboardComponent()->GetValueAsFloat(TEXT("Distance"))};
	IFighter* fighterRef{ Cast<IFighter>(OwnerComp.GetAIOwner()->GetCharacter())};
	if (Distance < fighterRef->GetMeleeRange())
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsEnum(TEXT("CurrentState"), EEnemyState::Melee);
		AbortTask(OwnerComp, NodeMemory);
		return EBTNodeResult::Aborted;
	}

	CharacterRef->PlayAnimMontage(m_AnimMontage);

	double randomValue{ UKismetMathLibrary::RandomFloat() };

	if (randomValue > m_threshhold)
	{
		m_threshhold = m_maxThreshhold;

		OwnerComp.GetBlackboardComponent()->SetValueAsEnum(TEXT("CurrentState"), EEnemyState::Charge);
	}
	else
	{
		m_threshhold -= m_failedAttemptReduce;
	}

	return EBTNodeResult::Succeeded;
}

UBTT_RANGED_ATTACK::UBTT_RANGED_ATTACK()
{
	bCreateNodeInstance = true;
}
