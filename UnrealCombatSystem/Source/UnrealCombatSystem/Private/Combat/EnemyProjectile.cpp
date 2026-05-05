// Fill out your copyright notice in the Description page of Project Settings.


#include "Combat/EnemyProjectile.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
UEnemyProjectile::UEnemyProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryComponentTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void UEnemyProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void UEnemyProjectile::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}


void UEnemyProjectile::SpawnProjectile(FName ComponentName, TSubclassOf<AActor> ProjectileClass)
{
	USceneComponent* SpawnPointComp{Cast<USceneComponent>(GetOwner()->GetDefaultSubobjectByName(ComponentName))};

	FVector SpawnLocation{ SpawnPointComp->GetComponentLocation() };

	FVector PlayerLocation{ GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation()};

	FRotator SpawnRotation{UKismetMathLibrary::FindLookAtRotation(SpawnLocation, PlayerLocation)};

	GetWorld()->SpawnActor(ProjectileClass,&SpawnLocation,&SpawnRotation);

}

