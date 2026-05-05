// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemyProjectileActor.generated.h"

UCLASS()
class UNREALCOMBATSYSTEM_API AEnemyProjectileActor : public AActor
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere)
	UParticleSystem* m_hitTemplate;

	UPROPERTY(EditAnywhere)
	float m_damage{ 10.0f };
public:	
	// Sets default values for this actor's properties
	AEnemyProjectileActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UFUNCTION(BlueprintCallable)
	void HandleBeginOverlap(AActor* otherActor);
	UFUNCTION()
	void DestroyProjectile();
};
