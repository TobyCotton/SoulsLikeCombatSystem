// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/BossCharacter.h"
#include "Character/StatsComponent.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Combat/CombatComponent.h"
#include "Character/MainCharacter.h"
#include "Character/StatsComponent.h"
#include "BrainComponent.h"
#include "Components/CapsuleComponent.h"



// Sets default values
ABossCharacter::ABossCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	m_statsComponent = CreateDefaultSubobject<UStatsComponent>(TEXT("Stats Component"));
	m_combatComponent = CreateDefaultSubobject<UCombatComponent>(TEXT("Combat Component"));
}

// Called when the game starts or when spawned
void ABossCharacter::BeginPlay()
{
	Super::BeginPlay();
	m_controllerRef = GetController<AAIController>();

	m_blackBoardComponent = m_controllerRef->GetBlackboardComponent();


	m_blackBoardComponent->SetValueAsEnum(TEXT("CurrentState"),m_initialState);

	GetWorld()->GetFirstPlayerController()->GetPawn<AMainCharacter>()->m_statsComponent->m_onZeroHealthDelegate.AddDynamic(this, &ABossCharacter::HandlePlayerDeath);

}

// Called every frame
void ABossCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABossCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ABossCharacter::DetectPawn(APawn* DetectedPawn, APawn* PawnToDetect)
{
	EEnemyState CurrentState{static_cast<EEnemyState>(m_blackBoardComponent->GetValueAsEnum(TEXT("CurrentState")))};

	if (DetectedPawn != PawnToDetect || CurrentState != EEnemyState::Idle) 
	{ 
		return; 
	}

	m_blackBoardComponent->SetValueAsEnum(TEXT("CurrentState"), EEnemyState::Range);

	UE_LOG(LogTemp, Warning, TEXT("PlayerDetected!"));
}

float ABossCharacter::GetDamage()
{
	return m_statsComponent->m_stats[EStat::Strength];
}

void ABossCharacter::Attack()
{
	m_combatComponent->RandomAttack();
}

float ABossCharacter::GetAnimDuration()
{
	return m_combatComponent->m_animDuration;
}

float ABossCharacter::GetMeleeRange()
{
	return m_statsComponent->m_stats[EStat::MeleeRange];
}

void ABossCharacter::HandlePlayerDeath()
{
	m_controllerRef->GetBlackboardComponent()->SetValueAsEnum(TEXT("CurrentState"), EEnemyState::GameOver);
}

void ABossCharacter::HandleDeath()
{
	float duration{ PlayAnimMontage(m_deathAnim) };

	m_controllerRef->GetBrainComponent()->StopLogic("defeated");

	FindComponentByClass<UCapsuleComponent>()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	FTimerHandle destroyTimerHandle;

	GetWorld()->GetTimerManager().SetTimer(destroyTimerHandle,this,&ABossCharacter::FinishDeathAnim, duration,false);

	IMainPlayer* playerRef{GetWorld()->GetFirstPlayerController()->GetPawn<IMainPlayer>()};

	if (!playerRef)
	{ 
		return; 
	}

	playerRef->EndLockonWithActor(this);

}

void ABossCharacter::FinishDeathAnim()
{
	Destroy();
}
