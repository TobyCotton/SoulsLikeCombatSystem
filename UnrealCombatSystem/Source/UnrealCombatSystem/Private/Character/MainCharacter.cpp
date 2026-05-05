// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/MainCharacter.h"
#include "Animations/PlayerAnimInstance.h"
#include "Character/StatsComponent.h"
#include "Character/EStat.h"
#include "Combat/LockOnComponent.h"
#include "Combat/TraceComponent.h"
#include "Combat/CombatComponent.h"
#include "Combat/BlockComponent.h"
#include "Character/PlayerActionsComponent.h"

// Sets default values
AMainCharacter::AMainCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	m_statsComponent = CreateDefaultSubobject<UStatsComponent>(TEXT("Stats Component"));
	m_blockComponent = CreateDefaultSubobject<UBlockComponent>(TEXT("Block Component"));
	m_combatComponent = CreateDefaultSubobject<UCombatComponent>(TEXT("Combat Component"));
	m_playerActionsComponent = CreateDefaultSubobject<UPlayerActionsComponent>(TEXT("Player Actions Component"));
	m_traceComponent = CreateDefaultSubobject<UTraceComponent>(TEXT("Trace Component"));
	m_lockOnComponent = CreateDefaultSubobject <ULockOnComponent> (TEXT("Lock on Component"));
}

// Called when the game starts or when spawned
void AMainCharacter::BeginPlay()
{
	Super::BeginPlay();
	m_playerAnim = Cast<UPlayerAnimInstance>(GetMesh()->GetAnimInstance());
}

// Called every frame
void AMainCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (m_statsComponent->GetStamina() < m_blockComponent->m_staminaCost)
	{
		m_playerAnim->m_isBlocking = false;
	}
}

// Called to bind functionality to input
void AMainCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

float AMainCharacter::GetDamage()
{
	return m_statsComponent->m_stats[EStat::Strength];
}

bool AMainCharacter::HasEnoughStamina(float Cost)
{
	return m_statsComponent->m_stats[EStat::Stamina] >= Cost;
}

void AMainCharacter::HandleDeath()
{
	PlayAnimMontage(m_deathAnimMontage);

	DisableInput(GetController<APlayerController>());

}

void AMainCharacter::EndLockonWithActor(AActor* ActorRef)
{
	if (m_lockOnComponent->m_currentTargetActor != ActorRef) 
	{ 
		return; 
	}

	m_lockOnComponent->EndLockon();
}

bool AMainCharacter::CanTakeDamage(AActor* Opponent)
{
	if (m_playerActionsComponent->m_isRollActive)
	{
		return false;
	}
	if (m_playerAnim->m_isBlocking)
	{
		return m_blockComponent->Check(Opponent);
	}

	return true;
}

void AMainCharacter::PlayHurtAnim()
{
	PlayAnimMontage(m_hurtAnimMontage);
}
