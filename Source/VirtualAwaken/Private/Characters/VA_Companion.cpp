// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/VA_Companion.h"
#include "Characters/VA_Character.h"

#pragma region INIT
// Sets default values
AVA_Companion::AVA_Companion()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AVA_Companion::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AVA_Companion::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (OwnerCharacter)
	{
		// Calculate position from player
		FVector TargetLoc = OwnerCharacter->GetActorLocation() + (OwnerCharacter->GetActorRotation().RotateVector(OffsetFromOwner));

		// Smooth interp
		FVector CurrentLoc = GetActorLocation();
		SetActorLocation(FMath::VInterpTo(CurrentLoc, TargetLoc, DeltaTime, FollowSpeed));

		// Always look to player rotation
		FRotator TargetRot = OwnerCharacter->GetActorRotation();
		SetActorRotation(FMath::RInterpTo(GetActorRotation(), TargetRot, DeltaTime, FollowSpeed));
	}

}
#pragma endregion

void AVA_Companion::ExecuteScan()
{
	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Cyan, TEXT("C0M-P4: Escaneando área..."));
}

void AVA_Companion::ExecuteOrder(int32 OrderID)
{

}
