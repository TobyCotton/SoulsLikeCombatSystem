// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_Charge.h"
#include "AiController.h"
#include "GameFramework/Character.h"
#include "Animations/BossAnimInstance.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Character/EEnemyState.h"
#include "GameFramework/CharacterMovementComponent.h"

UBTT_Charge::UBTT_Charge()
{
	bNotifyTick = true;
	m_moveCompletedDelegate.BindUFunction(this,"HandleMoveCompleted");
	bCreateNodeInstance = true;
}

EBTNodeResult::Type UBTT_Charge::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	m_controllerRef = OwnerComp.GetAIOwner();
	m_isTaskFinished = false;
	m_character = m_controllerRef->GetCharacter();
	m_bossAnim = Cast<UBossAnimInstance>(m_character->GetMesh()->GetAnimInstance());

	m_bossAnim->m_isCharging = true;
	OwnerComp.GetBlackboardComponent()->SetValueAsBool(TEXT("IsReadyToCharge"), false);

	return EBTNodeResult::InProgress;
}

void UBTT_Charge::ChargeAtPlayer()
{
	APawn* playerRef{GetWorld()->GetFirstPlayerController()->GetPawn()};
	FVector playerLocation{ playerRef->GetActorLocation() };

	FAIMoveRequest moveRequest{ playerLocation };
	moveRequest.SetUsePathfinding(true);
	moveRequest.SetAcceptanceRadius(m_acceptableRadius);

	m_controllerRef->MoveTo(moveRequest);
	m_controllerRef->SetFocus(playerRef);

	m_controllerRef->ReceiveMoveCompleted.AddUnique(m_moveCompletedDelegate);

	m_originalSpeed = m_character->GetCharacterMovement()->MaxWalkSpeed;
	m_character->GetCharacterMovement()->MaxWalkSpeed = m_chargeWalkSpeed;
}

void UBTT_Charge::HandleMoveCompleted()
{
	m_bossAnim->m_isCharging = false;
	FTimerHandle attackTimerHandle;
	m_character->GetWorldTimerManager().SetTimer(attackTimerHandle, this, &UBTT_Charge::FinishAttackTask, 1.0f,false);
	m_character->GetCharacterMovement()->MaxWalkSpeed = m_originalSpeed;
}

void UBTT_Charge::FinishAttackTask()
{
	m_isTaskFinished = true;
}

void UBTT_Charge::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	bool readyToCharge = OwnerComp.GetBlackboardComponent()->GetValueAsBool(TEXT("IsReadyToCharge"));
	if (readyToCharge)
	{
		readyToCharge = false;
		OwnerComp.GetBlackboardComponent()->SetValueAsBool(TEXT("IsReadyToCharge"), false);

		ChargeAtPlayer();
	}
	if (!m_isTaskFinished)
	{
		return;
	}
	OwnerComp.GetBlackboardComponent()->SetValueAsEnum(TEXT("CurrentState"), EEnemyState::Melee);

	m_controllerRef->ReceiveMoveCompleted.Remove(m_moveCompletedDelegate);
	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
}
