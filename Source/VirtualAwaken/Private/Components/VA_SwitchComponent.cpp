// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/VA_SwitchComponent.h"
#include "Interfaces/VA_InteractableInterface.h"
#include "TimerManager.h"

#pragma region INIT
// Sets default values for this component's properties
UVA_SwitchComponent::UVA_SwitchComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UVA_SwitchComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UVA_SwitchComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}
#pragma endregion

#pragma region MAIN FUNCTIONS
void UVA_SwitchComponent::ToggleSwitch()
{
  if (!bCanInteract) return;

	if (bIsOneTimeActivation && bIsOn) return;

  bIsOn = !bIsOn;
  NotifyTargets();

	// Reset
	if (bShouldReset && bIsOn)
	{
		GetWorld()->GetTimerManager().SetTimer(TH_Reset, this, &UVA_SwitchComponent::ToggleSwitch, ResetDelay);
	}
}

void UVA_SwitchComponent::NotifyTargets()
{
	for (AActor* Target : TargetActors)
	{
		if (Target && Target->GetClass()->ImplementsInterface(UVA_InteractableInterface::StaticClass()))
		{
      IVA_InteractableInterface::Execute_OnInteract(Target, GetOwner());
		}
	}
}
#pragma  endregion
