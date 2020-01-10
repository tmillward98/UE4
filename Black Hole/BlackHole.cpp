// Fill out your copyright notice in the Description page of Project Settings.


#include "BlackHole.h"
#include "Components/SphereComponent.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "DrawDebugHelpers.h"

// Sets default values
ABlackHole::ABlackHole()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Static mesh doesn't need collisison
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RootComponent = MeshComp;

	//Check for overlap events
	PullZone = CreateDefaultSubobject<USphereComponent>(TEXT("PullZone"));
	PullZone->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	PullZone->SetCollisionResponseToAllChannels(ECR_Overlap);
	PullZone->InitSphereRadius(500.0f);
	PullZone->SetupAttachment(RootComponent);

	DeadZone = CreateDefaultSubobject<USphereComponent>(TEXT("DeadZone"));
	DeadZone->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	DeadZone->SetCollisionResponseToAllChannels(ECR_Overlap);
	DeadZone->InitSphereRadius(100.0f);
	DeadZone->SetupAttachment(RootComponent);

	LineTracker = CreateDefaultSubobject<UBoxComponent>(TEXT("LineTracker"));
	LineTracker->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	LineTracker->InitBoxExtent(FVector(500,0,0));
	LineTracker->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ABlackHole::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABlackHole::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	DrawDebugSphere(GetWorld(), GetActorLocation(), PullZone->GetScaledSphereRadius(), 20, FColor::Purple, false, -1, 0, 1);
	DrawDebugSphere(GetWorld(), GetActorLocation(), DeadZone->GetScaledSphereRadius(), 20, FColor::Black, false, -1, 0, 1);
	DrawDebugBox(GetWorld(), GetActorLocation(), LineTracker->GetScaledBoxExtent(), FColor::Orange, false, -1, 0, -1);

	//Pull the objects in if they are within the pull zone
	PullZone->GetOverlappingComponents(OverlappingObjects);
	for (UPrimitiveComponent* a : OverlappingObjects) {
		if (a != NULL && a != this->DeadZone && a!= this->LineTracker) {
			//Shrink the actor as it gets pulled to the centre, the closer to the centre the object the stronger the force
			a->AddForce((PullZone->GetComponentLocation() - a->GetComponentLocation())*2*a->GetMass()*(FVector::Dist(PullZone->GetComponentLocation(), a->GetComponentLocation()) / PullZone->GetScaledSphereRadius()));
			a->GetAttachmentRootActor()->SetActorScale3D(a->GetAttachmentRootActor()->GetActorScale3D()*0.99);
			
		}
	}

	//If the objects are within the dead zone, destroy
	DeadZone->GetOverlappingComponents(OverlappingObjects);
	for (UPrimitiveComponent* a : OverlappingObjects) {
		if (a != NULL && a != this->LineTracker && a != this->PullZone) {
			a->GetAttachmentRootActor()->Destroy();
		}	
	}


	
}

