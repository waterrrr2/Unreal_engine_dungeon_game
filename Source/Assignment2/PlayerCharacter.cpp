// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"

#include "ItemActor.h"
#include "EnemyCharacter.h"
#include "Assignment2GameMode.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Attach a sword mesh to the character mesh
	SwordMesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("SwordMesh"));
	SwordMesh->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, FName("Hand_R"));
	
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	FScriptDelegate OnHitDelegate;
	OnHitDelegate.BindUFunction(this, FName("SwordSwing"));
	SwordMesh->OnComponentBeginOverlap.Add(OnHitDelegate);

	FScriptDelegate PickUpDelegate;
	PickUpDelegate.BindUFunction(this, FName("ItemPickup"));
	OnActorBeginOverlap.Add(PickUpDelegate);
	


}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);


}

void APlayerCharacter::SwordSwing(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//sets the damage range
	int damage = FMath::RandRange(20, 30);

	if (IsAttacking())
	{
		AEnemyCharacter* Enemy = Cast<AEnemyCharacter>(OtherActor);
		UCapsuleComponent* EnemyCapsule = Cast<UCapsuleComponent>(OtherComp);

		if (Enemy && EnemyCapsule)
		{
			if (AGameModeBase* GameMode = UGameplayStatics::GetGameMode(this))
			{
				// Cast it to our specific game mode
				if (AAssignment2GameMode* AssignmentGameMode = Cast<AAssignment2GameMode>(GameMode))
				{
					if (EnemyCapsule != nullptr)
					{
						StopAttacking();
						Enemy->takeDamage(damage);
						// Send a basic message
						FString NewMessage = FString::Printf(TEXT("I did %i damage!"),damage);
						AssignmentGameMode->SendCharacterMessage(NewMessage);

					}
				}
			}
		}
	}
}

void APlayerCharacter::ItemPickup(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AItemActor* Item = Cast<AItemActor>(OtherActor);

	FStringFormatNamedArguments Arguments;
	
	FItemStruct Items;
	Items.ItemName = "Coins";

	Arguments.Add(TEXT("ITEMNAME"), Items.ItemName);

	if (Item)
	{
		if (AGameModeBase* GameMode = UGameplayStatics::GetGameMode(this))
		{
			// Cast it to our specific game mode
			if (AAssignment2GameMode* AssignmentGameMode = Cast<AAssignment2GameMode>(GameMode))
			{
				if (Item != nullptr)
				{

					Item->Destroy();
					// Send a basic message
					FString NewMessage = FString::Format(TEXT("Congrats! You found {ITEMNAME}. \n Thanks for playing!"),Arguments);
					AssignmentGameMode->SendCharacterMessage(NewMessage);
					

					//Ends the game after 5 seconds after picking up the item. comment out the two lines to stop auto end game

					FTimerHandle TimerHandle;
					GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &APlayerCharacter::EndGame, 5, false);
				}
			}
		}
	}
	
}

//Ends the game
void APlayerCharacter::EndGame()
{
	FGenericPlatformMisc::RequestExit(false);
}
