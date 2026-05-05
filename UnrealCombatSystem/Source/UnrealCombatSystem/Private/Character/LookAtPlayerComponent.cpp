// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/LookAtPlayerComponent.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values for this component's properties
ULookAtPlayerComponent::ULookAtPlayerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void ULookAtPlayerComponent::BeginPlay()
{
	Super::BeginPlay();

	
	
}


// Called every frame
void ULookAtPlayerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	if (!m_bCanRotate)
	{
		return;
	}
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (!IsValid(m_owner))
	{
		m_owner = GetOwner();
	}
	FVector ownerLocation{m_owner->GetActorLocation()};
	if (!IsValid(m_playerPawn))
	{
		m_playerPawn = GetWorld()->GetFirstPlayerController()->GetPawn();
	}
	FVector playerLocation{ m_playerPawn->GetActorLocation() };
	FRotator desiredLocation{ UKismetMathLibrary::FindLookAtRotation(ownerLocation,playerLocation) };
	FRotator currentRotation{ m_owner->GetActorRotation() };
	FRotator newRotation{ UKismetMathLibrary::RInterpTo_Constant(currentRotation,desiredLocation,DeltaTime,m_rotationSpeed) };
	m_owner->SetActorRotation(FRotator{ currentRotation.Pitch,newRotation.Yaw,currentRotation.Roll });
}

