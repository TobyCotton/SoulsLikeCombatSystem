// Fill out your copyright notice in the Description page of Project Settings.


#include "Combat/BlockComponent.h"
#include "GameFramework/Character.h"
#include "Interfaces/MainPlayer.h"


// Sets default values for this component's properties
UBlockComponent::UBlockComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UBlockComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UBlockComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

bool UBlockComponent::Check(AActor* Opponent)
{
	ACharacter* characterRef{ GetOwner<ACharacter>() };

	if (!characterRef->Implements<UMainPlayer>()) 
	{ 
		return true; 
	}

	IMainPlayer* playerRef{ Cast<IMainPlayer>(characterRef) };

	FVector opponentForward{ Opponent->GetActorForwardVector() };
	FVector playerForward{ characterRef->GetActorForwardVector() };

	double Result{ FVector::DotProduct(opponentForward, playerForward) };

	if (Result > 0 || !playerRef->HasEnoughStamina(m_staminaCost))
	{
		return true;
	}

	characterRef->PlayAnimMontage(m_blockAnimMontage);

	m_onBlockDelegate.Broadcast(m_staminaCost);


	return false;
}


