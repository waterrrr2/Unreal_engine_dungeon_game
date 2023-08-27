// Fill out your copyright notice in the Description page of Project Settings.




#include "EnemyCharacter.h"
#include "Components/SphereComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
AEnemyCharacter::AEnemyCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PlayerDetectionSphere = CreateDefaultSubobject<USphereComponent>(FName("PlayerDetectionSphere"));
	AttackRadiusSphere = CreateDefaultSubobject<USphereComponent>(FName("AttackRadiusSphere"));

	if (RootComponent)
	{
		if (AttackRadiusSphere)
		{
			AttackRadiusSphere->SetSphereRadius(AttackRadius);

			// Make sure it's attached to the root component
			AttackRadiusSphere->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
		}
		
		if (PlayerDetectionSphere)
		{
			PlayerDetectionSphere->SetSphereRadius(PlayerDetectionRadius);

			// Make sure it's attached to the root component
			PlayerDetectionSphere->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
		}
	}

}

// Called when the game starts or when spawned
void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	FScriptDelegate ChasePlayerDelegate;
	ChasePlayerDelegate.BindUFunction(this, FName("ChasePlayer"));

	FScriptDelegate StopChasingDelegate;
	StopChasingDelegate.BindUFunction(this, FName("StopChasingPlayer"));


	PlayerDetectionSphere->OnComponentBeginOverlap.Add(ChasePlayerDelegate);

	PlayerDetectionSphere->OnComponentEndOverlap.Add(StopChasingDelegate);

	AttackRadiusSphere->OnComponentBeginOverlap.Add(StopChasingDelegate);

	AttackRadiusSphere->OnComponentEndOverlap.Add(ChasePlayerDelegate);
	

}

// Called every frame
void AEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	

	if (status == Chasing)
	{
		this->GetMovementComponent()->AddInputVector(DistanceBetween);
	}
	
}

// Called to bind functionality to input
void AEnemyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}


void AEnemyCharacter::takeDamage(int DamageAmount)
{
	Health -= DamageAmount;

	if (Health <= 0 && isAlive)
	{
		OnEnemyEliminated.ExecuteIfBound();
		isAlive = false;
		this->Destroy();
	}
}


void AEnemyCharacter::ChasePlayer(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	PlayerRef = Cast<APlayerCharacter>(OtherActor);

	if (PlayerRef)
	{
		FVector PlayerLocation = PlayerRef->GetActorLocation();
		FVector EnemyLocation = this->GetActorLocation();
		DistanceBetween = PlayerLocation - EnemyLocation;
		status = Chasing;
	}

}
void AEnemyCharacter::StopChasingPlayer(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	PlayerRef = Cast<APlayerCharacter>(OtherActor);

	if (PlayerRef)
	{
		DistanceBetween = EmptyVector;
		status = Stopped;
	}
}
