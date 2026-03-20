// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/VA_InteractionComponent.h"
#include "Interfaces/VA_InteractableInterface.h"
#include "Widgets/VA_HUD.h"
#include "Engine/OverlapResult.h"
#include "Environment/VA_MovingActor.h"
#include "Core/Dialogues/VA_DialogueManager.h"

#pragma region INIT
// Sets default values for this component's properties
UVA_InteractionComponent::UVA_InteractionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UVA_InteractionComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UVA_InteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

  BestCandidate = FindBestCandidate();

  // If enabled, draw debug information about the search radius and the best candidate interactable actor. This will help visualize how the interaction system is working and identify any issues with the search or line of sight checks.
	if (bDrawDebug)
	{
    FVector ViewLoc;
    FRotator ViewRot;
    GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(ViewLoc, ViewRot);

		if (BestCandidate)
		{
			DrawDebugLine(GetWorld(), ViewLoc, BestCandidate->GetActorLocation(), FColor::Blue);
    }
	}

  // Update the HUD with the new best candidate interactable actor. This will allow the HUD to show a prompt or highlight the actor if it's valid.
	if (APlayerController* PC = Cast<APlayerController>(GetWorld()->GetFirstPlayerController()))
	{
		if (AVA_HUD* HUD = Cast<AVA_HUD>(PC->GetHUD()))
		{
			UpdateHUD(HUD);
		}
	}
}
#pragma endregion

#pragma region OBJECTS

TObjectPtr<AActor> UVA_InteractionComponent::FindBestCandidate()
{
	UVA_DialogueManager* DM = GetWorld()->GetGameInstance()->GetSubsystem<UVA_DialogueManager>();
	if (DM && DM->IsDialogueActive())
	{
		return BestCandidate = nullptr;
	}

	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if (!PC && !PC->GetPawn()) return nullptr;

	FVector CharacterLoc = GetOwner()->GetActorLocation();

	FVector ViewLoc;
	FRotator ViewRot;
	PC->GetPlayerViewPoint(ViewLoc, ViewRot);
	FVector ViewDir = ViewRot.Vector();

	TArray<FOverlapResult> Hits;
	FCollisionShape Sphere = FCollisionShape::MakeSphere(SearchRadius);
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(GetOwner());

	// Scan by volume
	bool bHit = GetWorld()->OverlapMultiByChannel(Hits, CharacterLoc, FQuat::Identity, ECC_Visibility, Sphere, Params);

	AActor* Winner = nullptr;
	float BestScore = -1.0f;

	for (const FOverlapResult& Hit : Hits)
	{
		AActor* Potential = Hit.GetActor();
		if (Potential && Potential->Implements<UVA_InteractableInterface>())
		{
			// Filter: if the object cant be interacted, ignore it
      if (AVA_MovingActor* MovingActor = Cast<AVA_MovingActor>(Potential))
      {
        if (!MovingActor->GetCanInteractDirectly()) continue;
      }

			// Filter: if the object hasn't assigned a row in the table, ignore it
			FName RowName = IVA_InteractableInterface::Execute_GetInteractionRowName(Potential);
			if (RowName.IsNone()) continue;


      FVector ToActor = (Potential->GetActorLocation() - CharacterLoc).GetSafeNormal();
      float Dot = FVector::DotProduct(ViewDir, ToActor);

      // Only consider actors in front of the player (Dot > 0) and with a clear line of sight
			if (Dot > 0.4f)
			{
				float Dist = FVector::Dist(CharacterLoc, Potential->GetActorLocation());

        // Points algorithm: higher dot (more centered) and lower distance will give a higher score.
				float Score = Dot / (Dist * 0.01f);
				if (Score > BestScore && IsLineOfSightClear(Potential))
				{
					BestScore = Score;
					Winner = Potential;
        }
			}
		}
	}
  return Winner;
}

bool UVA_InteractionComponent::IsLineOfSightClear(AActor* Target)
{
  FHitResult OutHit;
  FVector Start = GetWorld()->GetFirstPlayerController()->PlayerCameraManager->GetCameraLocation();
  FVector End = Target->GetActorLocation();

  FCollisionQueryParams Params;
  Params.AddIgnoredActor(GetOwner());
	Params.AddIgnoredActor(Target);

  // If trace didn't hit anything between the camera and the target, it means we have a clear line of sight. We ignore the player and the target actors to avoid false positives.
  return !GetWorld()->LineTraceSingleByChannel(OutHit, Start, End, ECC_Visibility, Params);
}


#pragma endregion


#pragma region FUNCTIONS
void UVA_InteractionComponent::PrimaryInteract()
{
	if (BestCandidate)
	{
    IVA_InteractableInterface::Execute_OnInteract(BestCandidate, GetOwner());
  }
}

void UVA_InteractionComponent::UpdateHUD(AVA_HUD* HUD)
{
	if (!HUD || !BestCandidate)
	{
		if (HUD) HUD->UpdateInteractionWidget(nullptr, FVA_InteractionData());
		return;
	}

	FName RowName = IVA_InteractableInterface::Execute_GetInteractionRowName(BestCandidate);
  FVA_InteractionRow* Row = InteractionDT->FindRow<FVA_InteractionRow>(RowName, TEXT("Context"));

	if (Row)
	{
		FVA_InteractionData Data;
		Data.ActionName = Row->ActionText;
		Data.KeyboardIcon = Row->KeyboardIcon;
		Data.GamepadIcon = Row->GamepadIcon;

		HUD->UpdateInteractionWidget(BestCandidate, Data);
	}
}

#pragma endregion
