// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/AI/BTT_MeleeAttack.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Interfaces/Fighter.h"
#include "GameFramework/Character.h"
#include "Aicontroller.h"
#include "Character/eEnemystate.h"

EBTNodeResult::Type UBTT_MeleeAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	m_isFinished = false;
	float Distance{ OwnerComp.GetBlackboardComponent()->GetValueAsFloat(TEXT("Distance")) };

	AAIController* aiRef{ OwnerComp.GetAIOwner() };
	if (Distance > m_attackRadius)
	{
		APawn* playerRef{ GetWorld()->GetFirstPlayerController()->GetPawn() };

		FAIMoveRequest MoveRequest{ playerRef };
		MoveRequest.SetUsePathfinding(true);
		MoveRequest.SetAcceptanceRadius(m_acceptableRadius);

		aiRef->MoveTo(MoveRequest);
		aiRef->SetFocus(playerRef);
		aiRef->ReceiveMoveCompleted.AddUnique(m_moveDelegate);

	}
	else
	{
		IFighter* fighterRef{ Cast<IFighter>(aiRef->GetCharacter()) };
		fighterRef->Attack();
		FTimerHandle attackTimerHandle;
		aiRef->GetCharacter()->GetWorldTimerManager().SetTimer(attackTimerHandle, this, &UBTT_MeleeAttack::FinishAttackTask, fighterRef->GetAnimDuration(), false);
	}
	return EBTNodeResult::InProgress;
}

void UBTT_MeleeAttack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	float Distance{ OwnerComp.GetBlackboardComponent()->GetValueAsFloat(TEXT("Distance")) };
	AAIController* aiRef{OwnerComp.GetAIOwner()};
	IFighter* fighterRef{ Cast<IFighter>(aiRef->GetCharacter()) };
	if (Distance > fighterRef->GetMeleeRange())
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsEnum(TEXT("CurrentState"), EEnemyState::Range);
		AbortTask(OwnerComp, NodeMemory);
		FinishLatentTask(OwnerComp, EBTNodeResult::Aborted);
		aiRef->StopMovement();
		aiRef->ClearFocus(EAIFocusPriority::Gameplay);
		aiRef->ReceiveMoveCompleted.Remove(m_moveDelegate);
	}
	if (!m_isFinished) {
		return;
	}

	OwnerComp.GetAIOwner()->ReceiveMoveCompleted.Remove(m_moveDelegate);

	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);

}

UBTT_MeleeAttack::UBTT_MeleeAttack()
{
	m_moveDelegate.BindUFunction(this, "FinishAttackTask");
	bCreateNodeInstance = true;
	bNotifyTick = true;
}

void UBTT_MeleeAttack::FinishAttackTask()
{
	m_isFinished = true;
}

