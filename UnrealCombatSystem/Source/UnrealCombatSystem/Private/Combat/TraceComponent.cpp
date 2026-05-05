// Fill out your copyright notice in the Description page of Project Settings.


#include "Combat/TraceComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Interfaces/Fighter.h"

// Sets default values for this component's properties
UTraceComponent::UTraceComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UTraceComponent::BeginPlay()
{
	Super::BeginPlay();

	m_skeletalComp = GetOwner()->FindComponentByClass<USkeletalMeshComponent>();
	
}


// Called every frame
void UTraceComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!m_bIsAttacking) { return; }

	TArray<FHitResult> AllResults;
	for (const FTraceSockets Socket: m_sockets)
	{
		FVector startSocketLocation{ m_skeletalComp->GetSocketLocation(Socket.m_start) };
		FVector endSocketLocation{ m_skeletalComp->GetSocketLocation(Socket.m_end) };
		FQuat shapeRotation{ m_skeletalComp->GetSocketQuaternion(Socket.m_rotation) };

		TArray<FHitResult> OutResults;
		double WeapoDistance = FVector::Distance(startSocketLocation, endSocketLocation);
		FVector BoxHalfExtent{ m_boxCollissionLength, m_boxCollissionLength, WeapoDistance };
		BoxHalfExtent /= 2; // BoxHalfExtent = BoxHalfExtent / 2;
		FCollisionShape Box{ FCollisionShape::MakeBox(BoxHalfExtent) };
		FCollisionQueryParams IgnoreParams{
			FName { TEXT("Ignore Params") },
			false,
			GetOwner()
		};
		UWorld* world = GetWorld();

		bool bHasFoundTargets{ world->SweepMultiByChannel(
			OutResults,
			startSocketLocation,
			endSocketLocation,
			shapeRotation,
			ECollisionChannel::ECC_GameTraceChannel1,
			Box,
			IgnoreParams
		) };

		for (FHitResult hit: OutResults)
		{
			AllResults.Add(hit);
		}

		if (m_debugMode)
		{
			FVector centerPoint{ UKismetMathLibrary::VLerp(startSocketLocation,endSocketLocation,0.5f) };
			UKismetSystemLibrary::DrawDebugBox(world, centerPoint, Box.GetExtent(), bHasFoundTargets ? FLinearColor::Blue : FLinearColor::Yellow, shapeRotation.Rotator(), 1.0f);
		}
	}

	if (AllResults.Num() == 0)
	{
		return;
	}
	float characterDamage{ 0.0f };
	IFighter* fighterRef{ Cast<IFighter>(GetOwner()) };

	if (fighterRef)
	{
		characterDamage = fighterRef->GetDamage();
	}

	FDamageEvent targetAttackedEvent;

	for (const FHitResult& hit: AllResults)
	{
		AActor* targetActor{ hit.GetActor() };
		if (m_targetsToIgnore.Contains(targetActor))
		{
			continue;
		}
		targetActor->TakeDamage(characterDamage, targetAttackedEvent, GetOwner()->GetInstigatorController(), GetOwner());

		m_targetsToIgnore.AddUnique(targetActor);
	}
}

void UTraceComponent::HandleResetAttack()
{
	m_targetsToIgnore.Empty();
}

