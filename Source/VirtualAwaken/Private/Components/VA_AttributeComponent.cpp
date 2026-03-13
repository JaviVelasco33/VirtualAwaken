// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/VA_AttributeComponent.h"

#pragma region INIT
// Sets default values for this component's properties
UVA_AttributeComponent::UVA_AttributeComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	Health = MaxHealth;
}

// Called when the game starts
void UVA_AttributeComponent::BeginPlay()
{
	Super::BeginPlay();

	Health = MaxHealth;	
}

// Called every frame
void UVA_AttributeComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}
#pragma endregion

#pragma region HEALTH
bool UVA_AttributeComponent::ApplyHealthChange(float Delta, AActor* Instigator)
{
	// If it's already dead or there's no real change, return
	if (!IsAlive() && Delta < 0.0f) return false;

	float OldHealth = Delta;
	Health = FMath::Clamp(Health + Delta, 0.0f, MaxHealth);

	// if there's a real change
	if (Health != OldHealth)
	{
		OnHealthChanged.Broadcast(Health, MaxHealth, Instigator);
		return true;
	}

	return false;
}
#pragma endregion