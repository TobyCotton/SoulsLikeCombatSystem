// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_RANGED_ATTACK.generated.h"

/**
 * 
 */
UCLASS()
class UNREALCOMBATSYSTEM_API UBTT_RANGED_ATTACK : public UBTTaskNode
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	UAnimMontage* m_AnimMontage;

	UPROPERTY(EditAnywhere)
	double m_maxThreshhold{ 0.9f };
	UPROPERTY(VisibleAnywhere)
	double m_threshhold{ m_maxThreshhold };
	UPROPERTY(EditAnywhere)
	double m_failedAttemptReduce{ 0.1f };
public:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp,uint8* NodeMemory) override;
	UBTT_RANGED_ATTACK();
};
