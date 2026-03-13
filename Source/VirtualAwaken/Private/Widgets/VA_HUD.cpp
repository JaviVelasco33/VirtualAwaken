// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/VA_HUD.h"
#include "Interfaces/VA_InteractableInterface.h"
#include "Widgets/VA_InteractionWidget.h"

void AVA_HUD::UpdateInteractionWidget(AActor* NewTarget, const FVA_InteractionData& InteractionData)
{
  if (!NewTarget)
  {
    if (InteractionWidgetInstance) InteractionWidgetInstance->SetVisibility(ESlateVisibility::Collapsed);
    return;
  }

  // If doesn't exist the instance, create it and add it to the viewport. Then, set it visible and update the data with the new target actor and his interaction data.
  if (!InteractionWidgetInstance && InteractionWidgetClass)
  {
    InteractionWidgetInstance = CreateWidget<UVA_InteractionWidget>(GetOwningPlayerController(), InteractionWidgetClass);
    if (InteractionWidgetInstance)
    {
      InteractionWidgetInstance->AddToViewport();
    }
  }

  if (InteractionWidgetInstance)
  {
    InteractionWidgetInstance->SetupWidget(NewTarget, InteractionData);

    if (InteractionWidgetInstance->GetVisibility() != ESlateVisibility::Visible)
    {
      InteractionWidgetInstance->SetVisibility(ESlateVisibility::Visible);
    }
  }
}
