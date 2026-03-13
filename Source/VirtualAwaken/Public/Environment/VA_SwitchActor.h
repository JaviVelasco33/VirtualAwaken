// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/VA_InteractableInterface.h"
#include "VA_SwitchActor.generated.h"

UCLASS()
class VIRTUALAWAKEN_API AVA_SwitchActor : public AActor, public IVA_InteractableInterface
{
	GENERATED_BODY()
	
#pragma region INIT
public:	
	// Sets default values for this actor's properties
	AVA_SwitchActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
#pragma endregion

#pragma region COMPONENTS
protected:
  // Root component. The SwitchMesh will be attached to this. It will be used as the reference point for the switch.
  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "VA | Components")
  TObjectPtr<USceneComponent> RootComp;

  // The visible mesh of the switch. It will be used to show the switch in the level and to play the animation when toggling it.
  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "VA | Components")
  TObjectPtr<UStaticMeshComponent> SwitchMesh;

  // Component that will handle the switch logic. It will be used to toggle the switch and to send the events to the linked actors.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "VA | Components")
  TObjectPtr<class UVA_SwitchComponent> SwitchComponent;

	/** Select type for this object */
  UPROPERTY(EditDefaultsOnly, Category = "VA | Interaction")
  EVA_InteractionType InteractionAction = EVA_InteractionType::Interact;
#pragma endregion

#pragma region INTERACTION
public:
	virtual void OnInteract_Implementation(AActor* Interactor) override;

	virtual FName GetInteractionRowName_Implementation() override;
#pragma endregion
};
