// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_Charge.generated.h"

/**
 * 
 */
UCLASS()
class UNREALCOMBATSYSTEM_API UBTT_Charge : public UBTTaskNode
{
	GENERATED_BODY()

	AAIController* m_controllerRef;
	ACharacter* m_character;
	class UBossAnimInstance* m_bossAnim;
	UPROPERTY(EditAnywhere)
	float m_acceptableRadius{200.0f};

	FScriptDelegate m_moveCompletedDelegate;

	float m_originalSpeed;
	UPROPERTY(EditAnywhere)
	float m_chargeWalkSpeed{ 2000.0f };

	bool m_isTaskFinished{ true };
public:
	UBTT_Charge();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp,uint8* NodeMemory) override;
	void ChargeAtPlayer();
	UFUNCTION()
	void HandleMoveCompleted();

	UFUNCTION()
	void FinishAttackTask();
protected:
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	
};
