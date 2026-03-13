// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "VA_MovementType.generated.h"

UENUM(BlueprintType)
enum class EVA_MovementType : uint8
{
  Translation UMETA(DisplayName = "Translation"),
  Rotation UMETA(DisplayName = "Rotation")
};
