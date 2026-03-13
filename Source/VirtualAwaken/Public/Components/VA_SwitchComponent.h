// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Core/Types/VA_InteractionData.h"
#include "VA_SwitchComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class VIRTUALAWAKEN_API UVA_SwitchComponent : public UActorComponent
{
	GENERATED_BODY()

#pragma region INIT
public:	
	// Sets default values for this component's properties
	UVA_SwitchComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
#pragma endregion
		
#pragma region VARIABLES
protected:
	// The switch is On?
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "VA | State")
	bool bIsOn = false;

	// Can interact with the switch?
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "VA | State")
	bool bCanInteract = true;

	// Can interact only one time?
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "VA | State")
	bool bIsOneTimeActivation = false;

	// Should return to default after a time?
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "VA | Reset")
	bool bShouldReset = false;

	// Time to reset in seconds
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "VA | Reset", meta = (EditCondition = "bShouldReset"))
	float ResetDelay = 2.0f;
#pragma endregion

#pragma region TARGETS
protected:
	// Array of actors who will receive the signal
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "VA | Targets")
	TArray<TObjectPtr<AActor>> TargetActors;
#pragma endregion

#pragma region MAIN FUNCTIONS
public:
	// Inverts the state and notifies the targets
	UFUNCTION(BlueprintCallable, Category = "VA | Switch")
	void ToggleSwitch();

protected:
	void NotifyTargets();

	// Timer for reset when should reset
	FTimerHandle TH_Reset;
};
#pragma endregion