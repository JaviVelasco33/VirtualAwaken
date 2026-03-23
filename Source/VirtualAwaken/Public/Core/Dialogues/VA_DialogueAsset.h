// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "VA_DialogueTypes.h"
#include "VA_DialogueAsset.generated.h"

/**
 * 
 */
UCLASS()
class VIRTUALAWAKEN_API UVA_DialogueAsset : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "VA | Dialogue")
	TArray<FVA_DialogueLine> Lines;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "VA | Dialogue")
  TArray<FVA_ChoicePoint> ChoicePoint;

  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "VA | Dialogue")
  FName CompletionFlag;
};
