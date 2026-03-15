// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Widgets/VA_DialogueWidget.h"
#include "VA_GameInstance.generated.h"

/**
 * 
 */
UCLASS()
class VIRTUALAWAKEN_API UVA_GameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:

  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "VA | Narrative")
  TMap<FName, bool> NarrativeFlags;

  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "VA | UI")
  TSubclassOf<class UVA_DialogueWidget> DialogueWidgetClass;

  UFUNCTION(BlueprintCallable)
  void SetNarrativeFlag(FName FlagName, bool bValue) {NarrativeFlags.FindOrAdd(FlagName) = bValue; }

  UFUNCTION(BlueprintCallable)
  bool GetNarrativeFlag(FName FlagName) const { return NarrativeFlags.FindRef(FlagName); }
};
