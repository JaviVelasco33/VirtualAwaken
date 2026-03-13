// Fill out your copyright notice in the Description page of Project Settings.


#include "Environment/VA_SwitchActor.h"
#include "Components/VA_SwitchComponent.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "Characters/VA_Character.h"

#pragma region INIT
// Sets default values
AVA_SwitchActor::AVA_SwitchActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

  RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("RootComp"));
	RootComponent = RootComp;

  SwitchMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SwitchMesh"));
	SwitchMesh->SetupAttachment(RootComp);

  SwitchComponent = CreateDefaultSubobject<UVA_SwitchComponent>(TEXT("SwitchComponent"));
}

// Called when the game starts or when spawned
void AVA_SwitchActor::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AVA_SwitchActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


#pragma endregion

#pragma region INTERACTION
void AVA_SwitchActor::OnInteract_Implementation(AActor* Interactor)
{
	if (SwitchComponent)
	{
    SwitchComponent->ToggleSwitch();
	}
}

FName AVA_SwitchActor::GetInteractionRowName_Implementation()
{
  switch (InteractionAction)
  {
  case EVA_InteractionType::Interact:
    return FName("Interact");
    break;
  case EVA_InteractionType::Open:
    return FName("Open");
    break;
  case EVA_InteractionType::Talk:
    return FName("Talk");
    break;
  case EVA_InteractionType::Press:
    return FName("Press");
    break;
  case EVA_InteractionType::Grab:
    return FName("Grab");
    break;
  default:
    return FName("Interact");
    break;
  }

}
#pragma endregion
