// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Core/Types/VA_InteractionData.h"
#include "VA_InteractionComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class VIRTUALAWAKEN_API UVA_InteractionComponent : public UActorComponent
{
	GENERATED_BODY()

#pragma region INIT
public:	
	// Sets default values for this component's properties
	UVA_InteractionComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
#pragma endregion

#pragma region CONFIG
protected:
  // Radius for searching interactable actors around the player.
  UPROPERTY(EditAnywhere, Category = "VA | Interaction")
	float SearchRadius = 150.0f;

  // Weight of the importance: how much influence the angle against the distance
  UPROPERTY(EditAnywhere, Category = "VA | Interaction")
  float AngleWeight = 0.6f;

  // If true, it will draw debug spheres and lines to visualize the search radius and the interactions.
  UPROPERTY(EditAnywhere, Category = "VA | Interaction")
  bool bDrawDebug = false;

  UPROPERTY(EditAnywhere, Category = "VA | Config")
  TObjectPtr<UDataTable> InteractionDT;
#pragma endregion

#pragma region OBJECTS
protected:
  // The best candidate interactable actor found in the current search. It will be updated every tick and used when the player tries to interact.
  UPROPERTY(BlueprintReadOnly, Category = "VA | Interaction")
  TObjectPtr<AActor> BestCandidate;

  // The player character that owns this component. It will be used as the reference point for searching interactable actors and checking line of sight.
  TObjectPtr<AActor> FindBestCandidate();

  // Check if there is a clear line of sight between the player and the target actor, ignoring the player itself. It will be used to validate the best candidate interactable actor.
  bool IsLineOfSightClear(AActor* Target);
#pragma endregion

#pragma region FUNCTIONS
public:
  // This function will be called when the player tries to interact (e.g., by pressing the interact button). It will check if there is a valid best candidate interactable actor and, if so, it will call the interaction logic on that actor (e.g., by calling an interface function or triggering an event).
  UFUNCTION(BlueprintCallable, Category = "VA | Interaction")
  void PrimaryInteract();

  // Getter for the best candidate interactable actor. It can be used by other components or blueprints to get information about the current interactable target (e.g., to show a UI prompt or highlight the actor).
  TObjectPtr<AActor> GetBestCandidate() const { return BestCandidate; }

  UFUNCTION(BlueprintCallable, Category = "VA | HUD")
  void UpdateHUD(AVA_HUD* HUD);
#pragma endregion		
};
