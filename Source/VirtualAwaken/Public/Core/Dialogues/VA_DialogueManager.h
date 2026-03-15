// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "VA_DialogueAsset.h"
#include "Widgets/VA_DialogueWidget.h"
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

  // Function for widget shows player's choices
  UFUNCTION(BlueprintCallable, Category = "VA | Dialogue")
  void SelectChoice(int32 ChoiceIndex);

protected:
	// Widget class
  //UPROPERTY(EditAnywhere, Category = "VA | Dialogue")
  //TSubclassOf<class UVA_DialogueWidget> DialogueWidgetClass;

private:
	UPROPERTY()
	class UVA_DialogueAsset* CurrentAsset = nullptr;

	UPROPERTY()
  UVA_DialogueWidget* ActiveWidget = nullptr;

	int32 CurrentIndex = -1;

  void DisplayCurrentLine();
};
