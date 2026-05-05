// Fill out your copyright notice in the Description page of Project Settings.


#include "Combat/EnemyProjectileActor.h"
#include "Particles/ParticleSystemComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"

// Sets default values
AEnemyProjectileActor::AEnemyProjectileActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AEnemyProjectileActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEnemyProjectileActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEnemyProjectileActor::HandleBeginOverlap(AActor* otherActor)
{
	APawn* pawnRef{ Cast<APawn>(otherActor) };
	if (!pawnRef->IsPlayerControlled())
	{
		return;
	}
	FindComponentByClass<UParticleSystemComponent>()->SetTemplate(m_hitTemplate);
	FindComponentByClass<UProjectileMovementComponent>()->StopMovementImmediately();
	
	FTimerHandle deathTimerHandle{};
	GetWorldTimerManager().SetTimer(deathTimerHandle, this, &AEnemyProjectileActor::DestroyProjectile,0.5f);
	FindComponentByClass<USphereComponent>()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	FDamageEvent projectileAttackEvent{};
	pawnRef->TakeDamage(m_damage, projectileAttackEvent, pawnRef->GetController(), this);
}

void AEnemyProjectileActor::DestroyProjectile()
{
	Destroy();
}

