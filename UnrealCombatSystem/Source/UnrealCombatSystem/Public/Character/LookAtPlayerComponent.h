// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LookAtPlayerComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UNREALCOMBATSYSTEM_API ULookAtPlayerComponent : public UActorComponent
{
	GENERATED_BODY()
private:
	AActor* m_owner;
	APawn* m_playerPawn;
	UPROPERTY(EditAnywhere)
	float m_rotationSpeed{400.0f};
public:	
	// Sets default values for this component's properties
	ULookAtPlayerComponent();

	UPROPERTY(VisibleAnywhere)
	bool m_bCanRotate{ false };
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
