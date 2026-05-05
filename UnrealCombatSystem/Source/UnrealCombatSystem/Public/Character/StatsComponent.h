// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/EStat.h"
#include "Components/ActorComponent.h"
#include "StatsComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_SPARSE_DELEGATE_OneParam(
	FOnHealthPercentUpdateSignature,
	UStatsComponent, m_OnHealthPercentUpdateDelegate,
	float, Percentage
);

DECLARE_DYNAMIC_MULTICAST_SPARSE_DELEGATE_OneParam(
	FOnStaminaPercentUpdateSignature,
	UStatsComponent, m_OnStaminaPercentUpdateDelegate,
	float, Percentage
);

DECLARE_DYNAMIC_MULTICAST_SPARSE_DELEGATE(
	FOnZeroHealthSignature,
	UStatsComponent, m_onZeroHealthDelegate
);




UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UNREALCOMBATSYSTEM_API UStatsComponent : public UActorComponent
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	double m_StaminaRegenRate{10.0};
	UPROPERTY(VisibleAnywhere)
	bool m_canRegen{ true };
	UPROPERTY(EditAnywhere)
	float m_staminaDelay{2.0f};

public:	
	// Sets default values for this component's properties
	UStatsComponent();
	UPROPERTY(EditAnywhere)
	TMap<TEnumAsByte<EStat>,float> m_stats;

	UPROPERTY(BlueprintAssignable)
	FOnHealthPercentUpdateSignature m_OnHealthPercentUpdateDelegate;

	UPROPERTY(BlueprintAssignable)
	FOnStaminaPercentUpdateSignature m_OnStaminaPercentUpdateDelegate;

	UPROPERTY(BlueprintAssignable)
	FOnZeroHealthSignature m_onZeroHealthDelegate;



protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	UFUNCTION(BlueprintCallable)
	void ReduceHealth(float damage, AActor* Opponent);
	UFUNCTION(BlueprintCallable)
	void ReduceStamina(float staminaCost);
	UFUNCTION(BlueprintCallable)
	void RegenStamina();
	UFUNCTION(BlueprintCallable)
	float GetStamina();

	UFUNCTION()
	void EnableRegeneration();

	UFUNCTION(BlueprintPure)
	float GetStatPercentage(EStat current,EStat max);
};
