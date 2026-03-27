// Fill out your copyright notice in the Description page of Project Settings.


#include "Environment/VA_StaticSneakArea.h"
#include "Components/StaticMeshComponent.h"
#include "Components/ShapeComponent.h"
#include "Components/SceneComponent.h"

AVA_StaticSneakArea::AVA_StaticSneakArea()
{
  MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
  MeshComp->SetupAttachment(RootComponent);
  MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}
