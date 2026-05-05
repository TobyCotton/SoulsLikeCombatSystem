// Fill out your copyright notice in the Description page of Project Settings.


#include "Combat/LockOnComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/SpringArmComponent.h"
#include "Interfaces/Enemy.h"

// Sets default values for this component's properties
ULockOnComponent::ULockOnComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void ULockOnComponent::BeginPlay()
{
	Super::BeginPlay();

	m_ownerRef = GetOwner<ACharacter>();
	m_controller = GetWorld()->GetFirstPlayerController();
	m_movementComp = m_ownerRef->GetCharacterMovement();
	m_springArm = m_ownerRef->FindComponentByClass<USpringArmComponent>();
}

void ULockOnComponent::ToggleLockon(float Radius)
{
	if (IsValid(m_currentTargetActor))
	{
		EndLockon();
	}
	else
	{
		StartLockon(Radius);
	}

}

void ULockOnComponent::StartLockon(float Radius)
{
	FHitResult OutResult;
	FVector CurrentLocation{ m_ownerRef->GetActorLocation() };
	FCollisionShape Sphere{ FCollisionShape::MakeSphere(Radius) };
	FCollisionQueryParams IgnorePrams{
		FName(TEXT("Ignore Query Params")),
		false,
		m_ownerRef
	};

	bool bHasFoundTarget{ GetWorld()->SweepSingleByChannel(
		OutResult,
		CurrentLocation,
		CurrentLocation,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel1,
		Sphere,
		IgnorePrams
	) };//Finds detected Actor

	if (!bHasFoundTarget) { return; }
	
	if (!OutResult.GetActor()->Implements<UEnemy>())
	{
		return;
	}
	m_currentTargetActor = OutResult.GetActor();

	m_controller->SetIgnoreLookInput(true);
	m_movementComp->bOrientRotationToMovement = false;
	m_movementComp->bUseControllerDesiredRotation = true;

	m_springArm->TargetOffset = FVector{ 0.0f,0.0f,100.0f };

	IEnemy::Execute_OnSelect(m_currentTargetActor);
	m_OnUpdatedTargetDelegate.Broadcast(m_currentTargetActor);
}

void ULockOnComponent::EndLockon()
{
	IEnemy::Execute_OnDeselect(m_currentTargetActor);
	m_currentTargetActor = nullptr;
	m_movementComp->bOrientRotationToMovement = true;
	m_movementComp->bUseControllerDesiredRotation = false;
	m_springArm->TargetOffset = FVector{ 0.0f,0.0f,0.0f };
	m_controller->ResetIgnoreLookInput();
	m_OnUpdatedTargetDelegate.Broadcast(m_currentTargetActor);
}



// Called every frame
void ULockOnComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!IsValid(m_currentTargetActor))
	{
		return;
	}
	//Rotate camera to our target while Locked on 
	FVector myLocation{ m_ownerRef->GetActorLocation() };
	FVector targetLocation{ m_currentTargetActor->GetActorLocation() };

	if (FVector::Distance(myLocation,targetLocation) > m_breakDistance)
	{
		EndLockon();
		return;
	}
	targetLocation.Z -= 125.f;
	FRotator newRotation{ UKismetMathLibrary::FindLookAtRotation(myLocation, targetLocation) };
	m_controller->SetControlRotation(newRotation);
}

