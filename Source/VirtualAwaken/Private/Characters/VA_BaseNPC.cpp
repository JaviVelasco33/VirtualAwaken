// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/VA_BaseNPC.h"
#include "Core/Dialogues/VA_DialogueManager.h"
#include "Core/VA_GameInstance.h"

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
	UVA_GameInstance* GI = Cast<UVA_GameInstance>(GetGameInstance());

	if (!GI || !DialogueAsset) return;

  // Check if the dialogue is already completed, if so, play a random bark, else, start the dialogue
  bool bDialogueCompleted = GI->GetNarrativeFlag(DialogueAsset->CompletionFlag);

	if (bDialogueCompleted)
	{
    // The principal dialogue is already completed, play a random bark
		PlayAmbientBark();
    return;
	}
	else
	{
    // The dialogue is not completed, start or advance the dialogue
		UVA_DialogueManager* DialogueManager = GI->GetSubsystem<UVA_DialogueManager>();
    if (DialogueManager) DialogueManager->HandleDialogueInteraction(DialogueAsset);
	}
}

void AVA_BaseNPC::PlayAmbientBark()
{
  if (AmbientBarks.Num() == 0) return;

	// Choose a random bark
  int32 RandomIndex = FMath::RandRange(0, AmbientBarks.Num() - 1);
  FText SelectedBark = AmbientBarks[RandomIndex];

	if (GEngine)
	{
    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, FString::Printf(TEXT("%s says: %s"), *GetName(), *SelectedBark.ToString()));
	}
}