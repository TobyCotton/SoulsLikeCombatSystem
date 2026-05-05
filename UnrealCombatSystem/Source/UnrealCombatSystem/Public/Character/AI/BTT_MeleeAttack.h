// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_MeleeAttack.generated.h"

/**
 * 
 */
UCLASS()
class UNREALCOMBATSYSTEM_API UBTT_MeleeAttack : public UBTTaskNode
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere)
	float m_attackRadius{200.0f};
	UPROPERTY(EditAnywhere)
	float m_acceptableRadius{100.0f};
	FScriptDelegate m_moveDelegate;

	bool m_isFinished{ false };
protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp,uint8* NodeMemory) override;

	virtual void TickTask(UBehaviorTreeComponent& OwnerComp,uint8* NodeMemory,float DeltaSeconds) override;
public:
	UBTT_MeleeAttack();

	UFUNCTION()
	void FinishAttackTask();


};
