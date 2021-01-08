// Created by Diego de Miguel Moreno


#include "OpenDoor.h"
#include "GameFramework/Actor.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"

// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();

	InitialYaw = GetOwner()->GetActorRotation().Yaw;
	CurrentYaw = InitialYaw;
	OpenAngle += InitialYaw;

	ActorThatOpens = GetWorld()->GetFirstPlayerController()->GetPawn();

	/*if (!PressurePlate)
	{
		UE_LOG(LogTemp, Error, TEXT("The actor %s has the OpenDoor component, but no Pressure Plate set."), *PressurePlate->GetHumanReadableName())
	}*/


	
}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (PressurePlate && PressurePlate->IsOverlappingActor(ActorThatOpens))
	{
		OpenDoor(DeltaTime);
		DoorLastOpened = GetWorld()->GetTimeSeconds();
	}
	else
	{
		if (GetWorld()->GetTimeSeconds() - DoorLastOpened > DoorClosingDelay)
		{
			CloseDoor(DeltaTime);
		}
	}
}

void UOpenDoor::OpenDoor(float DeltaTime)
{
	CurrentYaw = FMath::FInterpTo(CurrentYaw, OpenAngle, DeltaTime, SpeedOpenDoor);
	FRotator DoorRotation(0, CurrentYaw, 0);
	GetOwner()->SetActorRotation(DoorRotation);

	// UE_LOG(LogTemp, Warning, TEXT("The yaw is %f degrees."), GetOwner()->GetActorRotation().Yaw);
}

void UOpenDoor::CloseDoor(float DeltaTime)
{
	CurrentYaw = FMath::FInterpTo(CurrentYaw, InitialYaw, DeltaTime, SpeedCloseDoor);
	FRotator DoorRotation(0, CurrentYaw, 0);
	GetOwner()->SetActorRotation(DoorRotation);

	// UE_LOG(LogTemp, Warning, TEXT("The yaw is %f degrees."), GetOwner()->GetActorRotation().Yaw);
}

