// Fill out your copyright notice in the Description page of Project Settings.


#include "Environment/VA_GasCloud.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/ShapeComponent.h"
#include "Components/SceneComponent.h"

AVA_GasCloud::AVA_GasCloud()
{
  SmokeParticles = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("SmokeParticles"));
  SmokeParticles->SetupAttachment(RootComponent);
  InitialLifeSpan = 8.0f;

}
