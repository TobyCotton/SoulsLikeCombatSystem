// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces\MainPlayer.h"
#include "Interfaces\Fighter.h"
#include "MainCharacter.generated.h"

UCLASS()
class UNREALCOMBATSYSTEM_API AMainCharacter : public ACharacter, public IMainPlayer, public IFighter
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	UAnimMontage* m_deathAnimMontage;
	UPROPERTY(EditAnywhere)
	UAnimMontage* m_hurtAnimMontage;


public:
	// Sets default values for this character's properties
	AMainCharacter();
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	class UStatsComponent* m_statsComponent;
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	class ULockOnComponent* m_lockOnComponent;
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	class UCombatComponent* m_combatComponent;
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	class UTraceComponent* m_traceComponent;
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	class UBlockComponent* m_blockComponent;
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	class UPlayerActionsComponent* m_playerActionsComponent;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UPROPERTY(BlueprintReadOnly)
	class UPlayerAnimInstance* m_playerAnim;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual float GetDamage() override;

	virtual bool HasEnoughStamina(float Cost) override;
	UFUNCTION(BlueprintCallable)
	void HandleDeath();

	virtual void EndLockonWithActor(AActor* ActorRef) override;

	virtual bool CanTakeDamage(AActor* Opponent) override;
	UFUNCTION(BlueprintCallable)
	void PlayHurtAnim();

};
