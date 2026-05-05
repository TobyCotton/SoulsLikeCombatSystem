// Fill out your copyright notice in the Description page of Project Settings.


#include "Combat/CombatComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetMathLibrary.h"
#include "Interfaces//MainPlayer.h"

// Sets default values for this component's properties
UCombatComponent::UCombatComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UCombatComponent::HandleResetAttack()
{
	m_bCanAttack = true;
}

void UCombatComponent::RandomAttack()
{
	int index = FMath::RandRange(0, m_attackAnimations.Num() - 1);
	if (index > m_attackAnimations.Num()-1)
	{
		UE_LOG(LogTemp,Warning,TEXT("iNDEX TO BIG"))
	}
	m_animDuration = m_character->PlayAnimMontage(m_attackAnimations[index]);
}


// Called when the game starts
void UCombatComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	m_character = GetOwner<ACharacter>();
}

void UCombatComponent::ComboAttack()
{
	if (m_character->Implements<UMainPlayer>())
	{
		IMainPlayer* iMainPlayerRef{ Cast<IMainPlayer>(m_character) };
		if (iMainPlayerRef && !iMainPlayerRef->HasEnoughStamina(m_StaminaCost))
		{
			return;
		}
	}
	if (m_bCanAttack)
	{
		m_bCanAttack = false;
		UE_LOG(
			LogTemp,
			Warning,
			TEXT("Combo")
		);
		m_character->PlayAnimMontage(m_attackAnimations[m_comboCounter]);
		m_comboCounter++;
		static int MaxCombo{ m_attackAnimations.Num() };
		m_comboCounter = UKismetMathLibrary::Wrap(m_comboCounter, -1, MaxCombo - 1);

		m_OnAttackPerformedDelegate.Broadcast(m_StaminaCost);
	}
}


// Called every frame
void UCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

