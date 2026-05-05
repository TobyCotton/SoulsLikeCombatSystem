// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/StatsComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Interfaces/Fighter.h"

// Sets default values for this component's properties
UStatsComponent::UStatsComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UStatsComponent::BeginPlay()
{
	Super::BeginPlay();
}


// Called every frame
void UStatsComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UStatsComponent::ReduceHealth(float damage, AActor* Opponent)
{
	if (m_stats[EStat::Health] < 0)
	{
		return;
	}

	IFighter* fighterRef{ GetOwner<IFighter>() };

	if (fighterRef && !fighterRef->CanTakeDamage(Opponent))
	{ 
		return; 
	}


	m_stats[EStat::Health] -= damage;
	m_stats[EStat::Health] = UKismetMathLibrary::FClamp(m_stats[EStat::Health], 0, m_stats[EStat::MaxHealth]);

	m_OnHealthPercentUpdateDelegate.Broadcast(GetStatPercentage(EStat::Health,EStat::MaxHealth));

	if (m_stats[EStat::Health] == 0)
	{
		m_onZeroHealthDelegate.Broadcast();
	}

}

void UStatsComponent::EnableRegeneration()
{
	m_canRegen = true;
}

float UStatsComponent::GetStatPercentage(EStat current, EStat max)
{
	if (m_stats[current] && m_stats[max])
	{
		return m_stats[current] / m_stats[max];
	}
	return -1.0f;
}

void UStatsComponent::ReduceStamina(float staminaCost)
{
	if (m_stats[EStat::Stamina] < 0)
	{
		return;
	}
	m_stats[EStat::Stamina] -= staminaCost;
	m_stats[EStat::Stamina] = UKismetMathLibrary::FClamp(m_stats[EStat::Stamina], 0, m_stats[EStat::MaxStamina]);
	m_canRegen = false;

	FLatentActionInfo functionInfo{ 0,100,TEXT("EnableRegeneration"),this };
	UKismetSystemLibrary::RetriggerableDelay(GetWorld(), m_staminaDelay, functionInfo);

	m_OnStaminaPercentUpdateDelegate.Broadcast(GetStatPercentage(EStat::Stamina, EStat::MaxStamina));
}

void UStatsComponent::RegenStamina()
{
	if (!m_canRegen)
	{
		return;
	}
	m_stats[EStat::Stamina] = UKismetMathLibrary::FInterpTo_Constant(m_stats[EStat::Stamina],m_stats[EStat::MaxStamina],GetWorld()->DeltaTimeSeconds,m_StaminaRegenRate);
	m_OnStaminaPercentUpdateDelegate.Broadcast(GetStatPercentage(EStat::Stamina, EStat::MaxStamina));
}

float UStatsComponent::GetStamina()
{
	return m_stats[EStat::Stamina];
}