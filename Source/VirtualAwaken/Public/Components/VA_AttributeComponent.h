// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "VA_AttributeComponent.generated.h"

// delegate to notify life changes
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnAttributeChanged, float, CurrenteValue, float, MaxValue, AActor*, Instigator);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class VIRTUALAWAKEN_API UVA_AttributeComponent : public UActorComponent
{
	GENERATED_BODY()

#pragma region INIT
public:	
	// Sets default values for this component's properties
	UVA_AttributeComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
#pragma endregion
		
#pragma region MAIN ATTRIBUTES
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attributes")
	float CurrentHealth;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attributes")
	float MaxHealth = 100.f;
#pragma endregion

#pragma region EVENTS
public:
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnAttributeChanged OnHealthChanged;
#pragma endregion

#pragma region CONTROL FUNCTIONS
public:
	UFUNCTION(BlueprintCallable, Category = "Attributes")
	bool ApplyHealthChange(float Delta, AActor* Instigator);

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	bool IsAlive() const { return CurrentHealth > 0.0f; }

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	float GetCurrentHealth() const { return CurrentHealth; }

		UFUNCTION(BlueprintCallable, Category = "Attributes")
	float GetMaxHealth() const { return MaxHealth; }

#pragma endregion
};
