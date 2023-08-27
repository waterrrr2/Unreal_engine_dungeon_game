// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyCharacter.h"
#include "ItemActor.h"
#include "GameFramework/Actor.h"
#include "SpawnerActor.generated.h"

UCLASS()
class ASSIGNMENT2_API ASpawnerActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASpawnerActor();

	//Binding the Enemy and Item blueprint to local variables
	UPROPERTY(EditAnywhere)
	TSubclassOf<AEnemyCharacter> EnemyToSpawn;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AItemActor> ItemToSpawn;

	UPROPERTY(EditAnywhere)
	int32 SpawnLimit;

	UPROPERTY(EditAnywhere)
	int32 LifeTimeLimit;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	void EnemyDies();

private:
	FTimerHandle SpawnTimerHandle;

	int EnemyCount = 0;

	int EnemyDeath = 0;
	
	void SpawnEnemy();

};
