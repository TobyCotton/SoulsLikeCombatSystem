// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FTraceSockets.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct UNREALCOMBATSYSTEM_API FTraceSockets
{
	GENERATED_BODY();
	UPROPERTY(EditAnywhere)
	FName m_start;
	UPROPERTY(EditAnywhere)
	FName m_end;
	UPROPERTY(EditAnywhere)
	FName m_rotation;
};
