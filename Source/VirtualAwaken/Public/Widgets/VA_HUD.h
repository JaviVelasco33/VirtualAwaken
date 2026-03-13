// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Core/Types/VA_InteractionData.h"
#include "VA_HUD.generated.h"

/**
 * 
 */
UCLASS()
class VIRTUALAWAKEN_API AVA_HUD : public AHUD
{
	GENERATED_BODY()
	
protected:
  // BP class that will be used to create the interaction widget instance. It should be set in the editor or in the constructor of the HUD class.
  UPROPERTY(EditAnywhere, Category = "VA | UI")
	TSubclassOf<class UVA_InteractionWidget> InteractionWidgetClass;

  UPROPERTY()
  TObjectPtr<class UVA_InteractionWidget> InteractionWidgetInstance;

public:
  // Show or hide the widget and update his data
  void UpdateInteractionWidget(AActor* NewTarget, const FVA_InteractionData& InteractionData);
};
