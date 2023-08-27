// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerCharacter.h"
#include "GameFramework/Character.h"
#include "EnemyCharacter.generated.h"


DECLARE_DELEGATE(FOnEnemyElimintated)


//Enum to store the current status of enemy
UENUM()
enum Status
{
	Chasing UMETA(DisplayName = "Chasing"),
	Stopped UMETA(DisplayName = "Stopped")
};

UCLASS()
class ASSIGNMENT2_API AEnemyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemyCharacter();

	// Bind to the Overlap and Endoverlap delegates of this component to run logic to go to the player
	UPROPERTY(EditAnywhere)
	class USphereComponent* PlayerDetectionSphere;

	// Bind to the Overlap and Endoverlap delegate of this component to run logic to attack the player
	UPROPERTY(EditAnywhere)
	class USphereComponent* AttackRadiusSphere;

	UPROPERTY(EditAnywhere)
	float AttackRadius = 100.f;

	UPROPERTY(EditAnywhere)
	float PlayerDetectionRadius = 500.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Status)
	TEnumAsByte<Status> status;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;



public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void takeDamage(int DamageAmount);

	UFUNCTION()
	void ChasePlayer(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void StopChasingPlayer(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);


public:
	FOnEnemyElimintated OnEnemyEliminated;

private:
	int Health = 100;
	bool isAlive = true;
	
	FVector DistanceBetween;
	FVector EmptyVector;
	APlayerCharacter* PlayerRef = nullptr;
	


	
};
