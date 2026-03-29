// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/VA_GameModeBase.h"
#include "Character/VA_Character.h"

#pragma region CONSTRUCTOR
AVA_GameModeBase::AVA_GameModeBase()
{
  // Search the character class and assign it as default
  static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Character/BP_VA_Character.BP_VA_Character_C"));

    if (PlayerPawnBPClass.Class != nullptr)
    {
      DefaultPawnClass =PlayerPawnBPClass.Class;
    }
}
#pragma endregion