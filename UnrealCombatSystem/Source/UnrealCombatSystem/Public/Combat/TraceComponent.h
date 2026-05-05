// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Combat\FTraceSockets.h"
#include "TraceComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UNREALCOMBATSYSTEM_API UTraceComponent : public UActorComponent
{
	GENERATED_BODY()

	USkeletalMeshComponent* m_skeletalComp;
	UPROPERTY(EditAnywhere)
	TArray<FTraceSockets> m_sockets;

	UPROPERTY(EditAnywhere)
	double m_boxCollissionLength{30.0f};

	UPROPERTY(EditAnywhere)
	bool m_debugMode{false};

	UPROPERTY(VisibleAnywhere)
	TArray<AActor*> m_targetsToIgnore;

public:	
	// Sets default values for this component's properties
	UTraceComponent();

	UFUNCTION(BlueprintCallable)
	void HandleResetAttack();
	UPROPERTY(VisibleAnywhere)
	bool m_bIsAttacking{ false };

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
