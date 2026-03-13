  // Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/VA_InteractionWidget.h"
#include "Components/TextBlock.h"
#include "CommonInputSubsystem.h"
#include "CommonInputTypeEnum.h"
#include "Components/Image.h"
#include "Framework/Application/SlateApplication.h"

#pragma region INIT
void UVA_InteractionWidget::NativeConstruct()
{
  Super::NativeConstruct();
}

void UVA_InteractionWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
  Super::NativeTick(MyGeometry, InDeltaTime);

  if (!TargetActor) return;

  APlayerController* PC = GetOwningPlayer();
  if (PC)
  {
    FVector2D ScreenPosition;

    // Project the world location of the target actor (plus the offset) to screen coordinates. If successful, update the widget's position in the viewport.
    if (PC->ProjectWorldLocationToScreen(TargetActor->GetActorLocation() + WorldOffset, ScreenPosition))
    {
      // Adjust the screen position to account for the widget's size, so that it is centered above the target actor.
      SetPositionInViewport(ScreenPosition);
    }
  }
}
#pragma endregion

#pragma region FUNCTIONS
void UVA_InteractionWidget::SetupWidget(AActor* InTarget, const FVA_InteractionData& InteractionData)
{
  TargetActor = InTarget;
  CurrentData = InteractionData;

  if (ActionTextBlock)
  {
    ActionTextBlock->SetText(InteractionData.ActionName);
  }

  UpdateIcon();
}

void UVA_InteractionWidget::UpdateIcon()
{
  if (!ActionIconImage) return;

  if (ULocalPlayer* LP = GetOwningLocalPlayer())
  {
    if (UCommonInputSubsystem* InputSubsystem = UCommonInputSubsystem::Get(LP))
    {
      ECommonInputType CurrentInput = InputSubsystem->GetCurrentInputType();
      bool bIsGamepad = (CurrentInput == ECommonInputType::Gamepad);

      UTexture2D* SelectedIcon = bIsGamepad ? CurrentData.GamepadIcon : CurrentData.KeyboardIcon;

      if (SelectedIcon)
      {
        ActionIconImage->SetBrushFromTexture(SelectedIcon);
      }
    }
  }
}
#pragma endregion