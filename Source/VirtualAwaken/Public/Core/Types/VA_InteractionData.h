// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "VA_InteractionData.generated.h"

USTRUCT(BlueprintType)
struct FVA_InteractionRow : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
	FText ActionText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
  TObjectPtr<UTexture2D> KeyboardIcon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
  TObjectPtr<UTexture2D> GamepadIcon;

};

USTRUCT(BlueprintType)
struct FVA_InteractionData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
	FText ActionName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
  TObjectPtr<UTexture2D> KeyboardIcon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
  TObjectPtr<UTexture2D> GamepadIcon;
};

UENUM(BlueprintType)
enum class EVA_InteractionType : uint8
{
	Interact UMETA(DisplayName = "Interact"),
	Open UMETA(DisplayName = "Open"),
	Talk UMETA(DisplayName = "Talk"),
	Press UMETA(DisplayName = "Press"),
  Grab UMETA(DisplayName = "Grab")
};