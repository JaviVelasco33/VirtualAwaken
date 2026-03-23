// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "VA_V311Phase.generated.h"

UENUM(BlueprintType)
enum class EVA_Phase : uint8
{
  Phase1 UMETA(DisplayName = "Phase I"),
  Phase2 UMETA(DisplayName = "Phase II"),
  Phase3 UMETA(DisplayName = "Phase III"),
  Phase4 UMETA(DisplayName = "Phase IV"),
};