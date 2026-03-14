// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "VA_NPCType.generated.h"

UENUM(BlueprintType)
enum class EVA_NPCType : uint8
{
  Ally UMETA(DisplayName = "Ally"),
  Neutral UMETA(DisplayName = "Neutral"),
  Enemy UMETA(DisplayName = "Enemy")
};