// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/Dialogues/VA_DialogueManager.h"
#include "Core/VA_GameInstance.h"
#include "Widgets/VA_DialogueWidget.h"
#include "Characters/VA_Character.h"
#include "Framework/Application/SlateApplication.h"

void UVA_DialogueManager::HandleDialogueInteraction(UVA_DialogueAsset* NewAsset, AVA_BaseNPC* InNPC)
{
  InteractedNPC = InNPC;

  if (!NewAsset) return;

  // If the dialogue is new, start it from the beginning
  if (CurrentAsset != NewAsset)
  {
    CurrentAsset = NewAsset;
    CurrentIndex = 0;
  }
  else
  {
    // If the dialogue is the same, advance to the next line
    CurrentIndex++;
  }

  // Check if we have reached the end of the dialogue
  if (CurrentIndex >= CurrentAsset->Lines.Num())
  {
    EndDialogue();
    return;
  }

  DisplayCurrentLine();

  // Lock Char movement during the dialogue and activate the orbit camera
  APlayerController* PC = GetWorld()->GetFirstPlayerController();
  if (PC)
  {
    // Lock movement and cam rotation
    PC->SetIgnoreMoveInput(true);
    PC->SetIgnoreLookInput(true);

    FInputModeGameAndUI InputMode;
    InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
    InputMode.SetHideCursorDuringCapture(false);
    PC->SetInputMode(InputMode);
    PC->bShowMouseCursor = true;

    AVA_Character* Player = Cast<AVA_Character>(PC->GetPawn());
    if (Player) Player->SetDialogueCameraMode(true);
  }


}

void UVA_DialogueManager::EndDialogue()
{
  AVA_Character* Player = Cast<AVA_Character>(GetWorld()->GetFirstPlayerController()->GetPawn());
  if (Player) Player->SetDialogueCameraMode(false);

  // Restore camera default controls
  APlayerController* PC = GetWorld()->GetFirstPlayerController();
  if (PC)
  {
    // Unlock movement and cam rotation
    PC->ResetIgnoreMoveInput();
    PC->ResetIgnoreLookInput();

    FInputModeGameAndUI InputMode;
    PC->SetInputMode(InputMode);
    PC->bShowMouseCursor = false;

    // Force the focus back to viewport
    PC->Activate(true);
  }

  if (ActiveWidget)
  {
    ActiveWidget->RemoveFromParent();
    ActiveWidget = nullptr;
  }

  if (CurrentAsset && !CurrentAsset->CompletionFlag.IsNone())
  {
    UVA_GameInstance* GI = Cast<UVA_GameInstance>(GetGameInstance());
    if (GI)
    {
      GI->SetNarrativeFlag(CurrentAsset->CompletionFlag, true);

      if (GEngine)
      {
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Saved Flag: %s"), *CurrentAsset->CompletionFlag.ToString()));
      }
    }
  }

  CurrentAsset = nullptr;
  CurrentIndex = -1;

  //bIsDialogueActive = false;
}

void UVA_DialogueManager::DisplayCurrentLine()
{
  if (!ActiveWidget)
  {
    UVA_GameInstance* GI = Cast<UVA_GameInstance>(GetGameInstance());
    if (GI && GI->DialogueWidgetClass)
    {
      ActiveWidget = CreateWidget<UVA_DialogueWidget>(GetWorld(), GI->DialogueWidgetClass);
      if (ActiveWidget)
      {
        ActiveWidget->AddToViewport();
        ActiveWidget->SetVisibility(ESlateVisibility::Visible);
      }
    }
  }

  if (!CurrentAsset || !CurrentAsset->Lines.IsValidIndex(CurrentIndex) || !ActiveWidget) return;

  const FVA_DialogueLine& Line = CurrentAsset->Lines[CurrentIndex];

  FText DisplayName;
  UTexture2D* DisplayPortrait = nullptr;

  // Choose what data send according to DataAsset Enum
  if (Line.Speaker == EVA_SpeakerType::NPC && InteractedNPC)
  {
    DisplayName = InteractedNPC->GetNPCName();
    DisplayPortrait = InteractedNPC->GetNPCPortrait();
  }
  else
  {
    UVA_GameInstance* GI = Cast<UVA_GameInstance>(GetGameInstance());
    if (GI)
    {
      DisplayName = GI->V311Name;
      DisplayPortrait = GI->V311Portrait;
    }
  }

  // Send all to the main widget
  ActiveWidget->OnUpdateDialogue(DisplayName, Line.Text);
  ActiveWidget->OnUpdateSpeaker(Line.Speaker, DisplayName, DisplayPortrait);
}

void UVA_DialogueManager::SelectChoice(int32 ChoiceIndex)
{
  if (!CurrentAsset || !CurrentAsset->Choices.IsValidIndex(ChoiceIndex)) return;

  const FVA_DialogueChoice& SelectedChoice = CurrentAsset->Choices[ChoiceIndex];

  // Apply the consequence of the choice, if any
  if (!SelectedChoice.ConsequenceFlag.IsNone())
  {
    UVA_GameInstance* GI = Cast<UVA_GameInstance>(GetGameInstance());
    if (GI)
    {
      GI->SetNarrativeFlag(SelectedChoice.ConsequenceFlag, true);
      if (GEngine)
      {
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Saved Flag: %s"), *SelectedChoice.ConsequenceFlag.ToString()));
      }
    }
  }

  // Go to the destination
  if (SelectedChoice.DestinationIndex != -1)
  {
    CurrentIndex = SelectedChoice.DestinationIndex;
    DisplayCurrentLine();
  }
  else
  {
    EndDialogue();
  }
}