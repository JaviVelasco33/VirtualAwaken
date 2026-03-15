// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/Dialogues/VA_DialogueManager.h"
#include "Core/VA_GameInstance.h"
#include "Widgets/VA_DialogueWidget.h"

void UVA_DialogueManager::HandleDialogueInteraction(class UVA_DialogueAsset* NewAsset)
{
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
}

void UVA_DialogueManager::EndDialogue()
{
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
}

void UVA_DialogueManager::DisplayCurrentLine()
{
  if (!CurrentAsset || !CurrentAsset->Lines.IsValidIndex(CurrentIndex)) return;

  if (!ActiveWidget)
  {
    UVA_GameInstance* GI = Cast<UVA_GameInstance>(GetGameInstance());
    if (GI && GI->DialogueWidgetClass)
    {
      ActiveWidget = CreateWidget<UVA_DialogueWidget>(GetWorld(), GI->DialogueWidgetClass);
      if (ActiveWidget)
      {
        ActiveWidget->AddToViewport();
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, TEXT("Widget de diálogo creado y añadido al Viewport."));
        ActiveWidget->SetVisibility(ESlateVisibility::Visible);
      }
      else
      {
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, TEXT("Fallo crítico al crear la instancia del Widget."));

      }
    }
    else
    {
      GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, TEXT("DialogueWidgetClass es NULL en el GameInstance. ¿Lo has asignado en el BP?"));
    }
  }

  if (ActiveWidget)
  {
    const FVA_DialogueLine& Line = CurrentAsset->Lines[CurrentIndex];

    ActiveWidget->OnUpdateDialogue(Line.SpeakerName, Line.Text);

  /*  if ((CurrentIndex == CurrentAsset->Lines.Num() - 1) && (CurrentAsset->Choices.Num() > 0))
    {
      ActiveWidget->OnShowChoices(CurrentAsset->Choices);
    }*/
    //// Format the message on Debug
    //FString DebugMessage = FString::Printf(TEXT("[%s]: %s"), *Line.SpeakerName.ToString(), *Line.Text.ToString());

    //// Display the dialogue line on Debug
    //if (GEngine)
    //{
    //  GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, DebugMessage);
    //}
  }
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