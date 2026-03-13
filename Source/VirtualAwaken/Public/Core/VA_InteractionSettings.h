// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Types/VA_InteractionData.h"
#include "VA_InteractionSettings.generated.h"

/**
 * 
 */
UCLASS()
class VIRTUALAWAKEN_API UVA_InteractionSettings : public UPrimaryDataAsset
{
  GENERATED_BODY()

public:
  /** Universal icon for keyboard (E key)*/
  UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Icon")
  TObjectPtr<UTexture2D> KeyboardIcon;

  /** Universal icon for gamepad (X button)*/
  UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Icon")
  TObjectPtr<UTexture2D> GamepadIcon;

  /** DataTable reference */
  UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Text")
  TObjectPtr<UDataTable> ActionDataTable;

  /** Utility function to get the text according to type */
  FText GetTextForAction(EVA_InteractionType Action) const;
};