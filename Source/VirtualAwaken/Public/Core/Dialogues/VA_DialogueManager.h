// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "VA_DialogueAsset.h"
#include "VA_DialogueManager.generated.h"

/**
 * 
 */
UCLASS()
class VIRTUALAWAKEN_API UVA_DialogueManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
  // Starts or advance a dialogue
  void HandleDialogueInteraction(class UVA_DialogueAsset* NewAsset);

  // Clear the current dialogue
	void EndDialogue();

private:
	UPROPERTY()
	class UVA_DialogueAsset* CurrentAsset = nullptr;

	int32 CurrentIndex = -1;

  void DisplayCurrentLine();
};
