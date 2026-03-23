// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Core/Dialogues/VA_DialogueTypes.h"
#include "VA_DialogueWidget.generated.h"

/**
 * 
 */
UCLASS()
class VIRTUALAWAKEN_API UVA_DialogueWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	// Events that will be filled in the Blueprint
  UFUNCTION(BlueprintImplementableEvent, Category = "VA | Dialogue")
	void OnUpdateDialogue(const FText& SpeakerName, const FText& DialogueText);

	UFUNCTION(BlueprintImplementableEvent, Category = "VA | Dialogue")
	void OnUpdateSpeaker(const EVA_SpeakerType& Speaker, const FText& SpeakerName, const UTexture2D* SpeakerPortrait);

  UFUNCTION(BlueprintImplementableEvent, Category = "VA | Dialogue")
	void OnShowChoices(const TArray<FVA_DialogueChoice>& Choices);

  UFUNCTION(BlueprintImplementableEvent, Category = "VA | Dialogue")
  void OnDialogueFinished();

	  UFUNCTION(BlueprintImplementableEvent, Category = "VA | Dialogue")
  void ClearChoices();
};
