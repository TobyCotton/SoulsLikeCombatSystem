// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/PlayerActionsComponent.h"
#include "Interfaces/MainPlayer.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values for this component's properties
UPlayerActionsComponent::UPlayerActionsComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UPlayerActionsComponent::BeginPlay()
{
	Super::BeginPlay();

	m_characterRef = GetOwner<ACharacter>();
	if (m_characterRef)
	{
		m_movementComponent = m_characterRef->GetCharacterMovement();
	}
	if (m_characterRef->Implements<UMainPlayer>())
	{
		m_iPlayerRef = Cast<IMainPlayer>(m_characterRef);
	}
}


// Called every frame
void UPlayerActionsComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UPlayerActionsComponent::Sprint()
{
	if (!m_iPlayerRef->HasEnoughStamina(m_sprintCost))
	{
		Walk();
		return;
	}
	if (m_movementComponent->Velocity.Equals(FVector::ZeroVector, 1)) { return; }
	m_movementComponent->MaxWalkSpeed = m_sprintSpeed;
	m_OnSprintDelegate.Broadcast(m_sprintCost);
}

void UPlayerActionsComponent::Walk()
{
	m_movementComponent->MaxWalkSpeed = m_originalSpeed;
}

void UPlayerActionsComponent::Roll()
{

	if (m_isRollActive || !m_iPlayerRef->HasEnoughStamina(m_rollCost))
	{
		return;
	}

	m_isRollActive = true;

	m_onRollDelegate.Broadcast(m_rollCost);

	FVector Direction{m_characterRef->GetCharacterMovement()->Velocity.Length() < 1 ?m_characterRef->GetActorForwardVector() :m_characterRef->GetLastMovementInputVector()};

	FRotator NewRot{ UKismetMathLibrary::MakeRotFromX(Direction) };

	m_characterRef->SetActorRotation(NewRot);

	float Duration{ m_characterRef->PlayAnimMontage(m_rollAnimMontage) };
	FTimerHandle RollTimerHandle;

	m_characterRef->GetWorldTimerManager().SetTimer(RollTimerHandle,this,&UPlayerActionsComponent::FinishRollAnim,Duration,false);
}

void UPlayerActionsComponent::FinishRollAnim()
{
	m_isRollActive = false;
}
