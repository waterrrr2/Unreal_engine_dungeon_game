// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/Actor.h"
#include "ItemActor.generated.h"


// struct to store item names
USTRUCT()
struct FItemStruct
{
	GENERATED_BODY()

	FString ItemName;
};

UCLASS(Blueprintable)
class ASSIGNMENT2_API AItemActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItemActor();

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* ItemMeshComponent;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
