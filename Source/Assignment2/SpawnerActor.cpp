// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawnerActor.h"

// Sets default values
ASpawnerActor::ASpawnerActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASpawnerActor::BeginPlay()
{
	Super::BeginPlay();

	FTimerDelegate TimerDelegate;
	TimerDelegate.BindUObject(this, &ASpawnerActor::SpawnEnemy);

	FTimerManager& TimerManager = GetWorldTimerManager();

	TimerManager.SetTimer(SpawnTimerHandle, TimerDelegate, 1.0f, true);
	
}

// Called every frame
void ASpawnerActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASpawnerActor::SpawnEnemy()
{
	if (EnemyToSpawn != nullptr && ItemToSpawn != nullptr)
	{
		if (UWorld* World = GetWorld())
		{

			FVector ESActorLoc = this->GetActorLocation();
			FRotator ESActorRot = this->GetActorRotation();
			

			//Checks if the number of enemies reaches the total spawn limit, if not, spawn enemy, if all enemy dies, spawn the item
			if (EnemyDeath+EnemyCount < LifeTimeLimit)
			{
				if (EnemyCount < SpawnLimit)
				{

					AEnemyCharacter* CastedEnemy = Cast<AEnemyCharacter>(World->SpawnActor<AActor>(EnemyToSpawn, ESActorLoc, ESActorRot));
					EnemyCount++;

					CastedEnemy->OnEnemyEliminated.BindUObject(this, &ASpawnerActor::EnemyDies);

				}
			}
			else
			{
				if (EnemyDeath == LifeTimeLimit)
				{
					AItemActor* CastedEnemy = Cast<AItemActor>(World->SpawnActor<AActor>(ItemToSpawn, ESActorLoc, ESActorRot));
					EnemyDeath++;
				}
			}
			
		}
	}
}

void ASpawnerActor::EnemyDies()
{
	EnemyCount--;
	EnemyDeath++;
}



