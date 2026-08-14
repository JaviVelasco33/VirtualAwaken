// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/VA_AttributeComponent.h"

#pragma region INIT
// Sets default values for this component's properties
UVA_AttributeComponent::UVA_AttributeComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	CurrentHealth = MaxHealth;
}

// Called when the game starts
void UVA_AttributeComponent::BeginPlay()
{
	Super::BeginPlay();

	// Get the component owner and suscribe the damage delegate
	AActor* MyOwner = GetOwner();
	if (MyOwner)
	{
		MyOwner->OnTakeAnyDamage.AddDynamic(this, &UVA_AttributeComponent::HandleTakeAnyDamage);
	}
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
	CurrentHealth = FMath::Clamp(CurrentHealth + Delta, 0.0f, MaxHealth);

	// if there's a real change
	if (CurrentHealth != OldHealth)
	{
		OnHealthChanged.Broadcast(CurrentHealth, MaxHealth, Instigator);
		return true;
	}

	return false;
}

void UVA_AttributeComponent::HandleTakeAnyDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	// If is empty damage or is already dead, return
	if (Damage <= 0.f || CurrentHealth <= 0.f) return;

	// Subtract the damage clamping the minimum health at 0
	CurrentHealth = FMath::Clamp(CurrentHealth - Damage, 0.f, MaxHealth);

	if (CurrentHealth <= 0.f)
	{
		DamagedActor->Destroy();
	}
}

#pragma endregion