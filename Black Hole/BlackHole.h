// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BlackHole.generated.h"

class USphereComponent;
class UBoxComponent;

/**
-- Black Hole --
Two sphere components should be used, one to indicate range of pull and the other to indicate destruction zone
When the object enters the pull range, it should have force applied to it pulling it in the direction of the black hole
The speed at which the object moves should increase base on distance to the destruction zone (Eg. closer to the destruction zone, the stronger the pull)
When the object enters the destruction zone, it's destroy function should be called 

Each tick we need to see which objects are in pull range, and add force to pull them towards the centre

**/

UCLASS()
class FPSGAME_API ABlackHole : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABlackHole();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UStaticMeshComponent* MeshComp;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	USphereComponent* PullZone;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	USphereComponent* DeadZone;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UBoxComponent* LineTracker;

	TArray<UPrimitiveComponent*> OverlappingObjects;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//Check to see if overlap occurs
	//virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

};
