// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Animation\AnimMontage.h"
#include "CombatComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_SPARSE_DELEGATE_OneParam(
	FonAttackPerformedSignature,
	UCombatComponent,
	m_OnAttackPerformedDelegate,
	float,Amount
);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UNREALCOMBATSYSTEM_API UCombatComponent : public UActorComponent
{
	GENERATED_BODY()

	ACharacter* m_character;
	UPROPERTY(VisibleAnywhere)
	int m_comboCounter{0};
	UPROPERTY(VisibleAnywhere)
	bool m_bCanAttack{true};
	UPROPERTY(EditAnywhere)
	float m_StaminaCost{ 5.0f };

public:	
	// Sets default values for this component's properties
	UCombatComponent();
	UPROPERTY(BlueprintAssignable)
	FonAttackPerformedSignature m_OnAttackPerformedDelegate;
	UPROPERTY(EditAnywhere)
	TArray<UAnimMontage*> m_attackAnimations;
	UFUNCTION(BlueprintCallable)
	void HandleResetAttack();
	void RandomAttack();

	float m_animDuration;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	UFUNCTION(BlueprintCallable)
	void ComboAttack();

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
