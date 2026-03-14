// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/Dialogues/VA_DialogueManager.h"

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
  if (GEngine)
  {
    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, TEXT("--- Fin de la conversación ---"));
  }
  CurrentAsset = nullptr;
  CurrentIndex = -1;
}

void UVA_DialogueManager::DisplayCurrentLine()
{
  if (!CurrentAsset || !CurrentAsset->Lines.IsValidIndex(CurrentIndex)) return;

  const FVA_DialogueLine& Line = CurrentAsset->Lines[CurrentIndex];

  // Format the message on Debug
  FString DebugMessage = FString::Printf(TEXT("[%s]: %s"), *Line.SpeakerName.ToString(), *Line.Text.ToString());

  // Display the dialogue line on Debug
  if (GEngine)
  {
    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, DebugMessage);
  }
}
