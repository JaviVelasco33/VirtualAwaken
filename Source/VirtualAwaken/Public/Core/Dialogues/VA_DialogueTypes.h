// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "VA_DialogueTypes.generated.h"

USTRUCT(BlueprintType)
struct FVA_DialogueLine
{
  GENERATED_BODY()

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VA | Dialogue")
  FText SpeakerName;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VA | Dialogue")
  FText Text;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VA | Dialogue")
  TSoftObjectPtr<USoundBase> VoiceAudio;

  // Optional flag that must be set for this dialogue to be available
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VA | Dialogue")
  FName RequiredFlag = NAME_None;
};

USTRUCT(BlueprintType)
struct FVA_DialogueChoice
{
  GENERATED_BODY()

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VA | Dialogue")
  FText ChoiceText;

  // Flag that activate when this choice is selected
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VA | Dialogue")
  FName ConsequenceFlag = NAME_None;

  // To what index this choice will be redirected
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VA | Dialogue")
  int32 DestinationIndex = -1;
};