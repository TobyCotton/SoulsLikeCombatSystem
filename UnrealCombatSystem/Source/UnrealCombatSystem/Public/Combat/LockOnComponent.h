// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LockOnComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_SPARSE_DELEGATE_OneParam(
	FOnUpdatedTargetSignature,
	ULockOnComponent, m_OnUpdatedTargetDelegate,
	AActor*,NewTargetActorRef
);


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UNREALCOMBATSYSTEM_API ULockOnComponent : public UActorComponent
{
	GENERATED_BODY()

	ACharacter* m_ownerRef;
	APlayerController* m_controller;
	class UCharacterMovementComponent* m_movementComp;

	class USpringArmComponent* m_springArm;

public:	
	// Sets default values for this component's properties
	ULockOnComponent();
	AActor* m_currentTargetActor;
	UPROPERTY(BlueprintAssignable)
	FOnUpdatedTargetSignature m_OnUpdatedTargetDelegate;
protected:
	UPROPERTY(EditAnywhere)
	double m_breakDistance = 1000.0f;
	// Called when the game starts
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void ToggleLockon(float Radius = 750.0f);
	void StartLockon(float Radius = 750.0f);

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void EndLockon();
		
};
