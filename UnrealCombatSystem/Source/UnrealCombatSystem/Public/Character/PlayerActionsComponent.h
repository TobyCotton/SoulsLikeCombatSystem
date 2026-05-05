// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlayerActionsComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_SPARSE_DELEGATE_OneParam(
	FOnSprintSignature,
	UPlayerActionsComponent, m_OnSprintDelegate,
	float, Cost
);
DECLARE_DYNAMIC_MULTICAST_SPARSE_DELEGATE_OneParam(
	FOnRollSignature,
	UPlayerActionsComponent, m_onRollDelegate,
	float, Cost
);


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UNREALCOMBATSYSTEM_API UPlayerActionsComponent : public UActorComponent
{
	GENERATED_BODY()
private:
	ACharacter* m_characterRef;
	class IMainPlayer* m_iPlayerRef;
	class UCharacterMovementComponent* m_movementComponent;
	UPROPERTY(EditAnywhere)
	float m_sprintCost{ 0.1f };
	UPROPERTY(EditAnywhere)
	float m_sprintSpeed{ 1000.0f };
	UPROPERTY(EditAnywhere)
	float m_originalSpeed{ 500.0f };
	UPROPERTY(EditAnywhere)
	UAnimMontage* m_rollAnimMontage;

	UPROPERTY(EditAnywhere)
	float m_rollCost{ 5.0f };


public:	
	// Sets default values for this component's properties
	UPlayerActionsComponent();

	UPROPERTY(BlueprintAssignable)
	FOnSprintSignature m_OnSprintDelegate;
	UPROPERTY(BlueprintAssignable)
	FOnRollSignature m_onRollDelegate;
	UPROPERTY(VisibleAnywhere)
	bool m_isRollActive{ false };
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	UFUNCTION(BlueprintCallable)
	void Sprint();
	UFUNCTION(BlueprintCallable)
	void Walk();

	UFUNCTION(BlueprintCallable)
	void Roll();

	UFUNCTION()
	void FinishRollAnim();

};
