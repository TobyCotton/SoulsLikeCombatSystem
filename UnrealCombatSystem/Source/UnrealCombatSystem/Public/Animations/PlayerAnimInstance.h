// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "PlayerAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class UNREALCOMBATSYSTEM_API UPlayerAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float m_currentSpeed{ 0.0f };
	UFUNCTION(BlueprintCallable)
	void UpdateSpeed();
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool m_bIsInCombat{ false };
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float m_CurrentDirection{ 0.0f };
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool m_isBlocking{ false };

	UFUNCTION(BlueprintCallable)
	void HandleUpdatedTarget(AActor* NewTargetActorRef);

	UFUNCTION(BlueprintCallable)
	void UpdateDirection();
};
