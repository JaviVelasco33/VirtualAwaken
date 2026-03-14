// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/VA_BaseNPC.h"
#include "Core/Dialogues/VA_DialogueManager.h"

#pragma region INIT
// Sets default values
AVA_BaseNPC::AVA_BaseNPC()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AVA_BaseNPC::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AVA_BaseNPC::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsRotatingToInteract)
	{
		FRotator CurrentRot = GetActorRotation();
		FRotator SmoothRot = FMath::RInterpTo(CurrentRot, TargetInteractRotation, DeltaTime, 5.f);
		SetActorRotation(SmoothRot);

		if (CurrentRot.Equals(TargetInteractRotation, 5.f))
		{
			bIsRotatingToInteract = false;
		}
	}
}

// Called to bind functionality to input
void AVA_BaseNPC::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}
#pragma endregion

FName AVA_BaseNPC::GetInteractionRowName_Implementation()
{
  return InteractionRowName;
}

void AVA_BaseNPC::OnInteract_Implementation(AActor* Interactor)
{
  if (!DialogueAsset) return;

	if (Interactor)
	{
		FVector Direction = Interactor->GetActorLocation() - GetActorLocation();
		Direction.Z = 0.f;
		TargetInteractRotation = Direction.Rotation();
		bIsRotatingToInteract = true;
	}
	ShowNextLine();
}

void AVA_BaseNPC::ShowNextLine()
{
	if (DialogueAsset)
	{
    UVA_DialogueManager* DialogueManager = GetGameInstance()->GetSubsystem<UVA_DialogueManager>();
		if (DialogueManager&& DialogueAsset)
		{
			DialogueManager->HandleDialogueInteraction(DialogueAsset);
		}
	}
}

