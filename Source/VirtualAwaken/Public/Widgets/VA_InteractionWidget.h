// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Core/Types/VA_InteractionData.h"
#include "VA_InteractionWidget.generated.h"

/**
 * 
 */
UCLASS()
class VIRTUALAWAKEN_API UVA_InteractionWidget : public UUserWidget
{
	GENERATED_BODY()
	
#pragma region INIT
protected:
  virtual void NativeConstruct() override;

  // Position update frame by frame to follow the target actor
  virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
#pragma endregion

#pragma region CONFIG
public:
  // These names should match with the variable names in the BP
  UPROPERTY(meta = (BindWidget))
  TObjectPtr<class UTextBlock> ActionTextBlock;

  UPROPERTY(meta = (BindWidget))
  TObjectPtr<class UImage> ActionIconImage;

  // The actor that the widget should follow in the screen
  UPROPERTY()
  TObjectPtr<AActor> TargetActor;

  // Offset from the target actor's location to position the widget in the world (e.g., above the actor's head)
  UPROPERTY(EditAnywhere, Category = "VA | UI")
  FVector WorldOffset = FVector(0.f, 0.f, 50.f);

protected:
  // Save the current interaction data to change the icon in the tick
  FVA_InteractionData CurrentData;
#pragma endregion

#pragma region FUNCTIONS
  public:
  // Sets up the widget with the given interaction data and target actor
	UFUNCTION(BlueprintCallable, Category = "VA | UI")
  void SetupWidget(AActor* InTarget, const FVA_InteractionData& InteractionData);

protected:
  // Function to decide which icon to show
  void UpdateIcon();
};
