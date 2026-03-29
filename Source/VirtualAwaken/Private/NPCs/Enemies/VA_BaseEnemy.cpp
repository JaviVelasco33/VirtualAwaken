// Fill out your copyright notice in the Description page of Project Settings.


#include "NPCs/Enemies/VA_BaseEnemy.h"

AVA_BaseEnemy::AVA_BaseEnemy()
{
  Tags.Add(FName("Enemy"));
  Tags.Add(FName("Scannable"));
}

void AVA_BaseEnemy::BeginPlay()
{
  Super::BeginPlay();

  GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red,
    FString::Printf(TEXT("Enemigo %s desplegado en el sector."), *GetName()));
}
