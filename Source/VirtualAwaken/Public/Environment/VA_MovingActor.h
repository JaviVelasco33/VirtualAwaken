// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/VA_InteractableInterface.h"
#include "Core/Types/VA_MovementType.h"
#include "Core/Types/VA_InteractionData.h"
#include "VA_MovingActor.generated.h"

UCLASS()
class VIRTUALAWAKEN_API AVA_MovingActor : public AActor, public IVA_InteractableInterface
{
	GENERATED_BODY()
	
#pragma region INIT
public:	
	// Sets default values for this actor's properties
	AVA_MovingActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

#pragma endregion

#pragma region COMPONENTS
protected:
  // Root component. The Pivot and the Mesh will be attached to this. It will be used as the reference point for the movement (initial position/rotation).
  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "VA | Components")
	TObjectPtr<USceneComponent> RootComp;

  // Pivot to make the rotation movement around a custom point. In translation, it will be the same as RootComp.
  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "VA | Components")
	TObjectPtr<USceneComponent> PivotComp;

  // The visible mesh of the actor. It will be moved/rotated by the PivotComp.
  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "VA | Components")
  TObjectPtr<UStaticMeshComponent> MeshComp;
#pragma endregion

#pragma region MOVEMENT CONFIG
  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "VA | Movement")
  EVA_MovementType MovementType = EVA_MovementType::Translation;

  // Point B (Relative). Point A is the actor's initial location/rotation.
  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "VA | Movement", meta = (MakeEditWidget = true))
  FVector FinalPosition;

  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "VA | Movement")
  float MoveSpeed = 1.0f;

  // Smoothness of the curve (1.0 = lineal, 2.0+ = Ease In/Out).
  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "VA | Movement")
  float EaseAmount = 3.0f;

  // If true, the player can interact by pressing "E". If false, only can be triggered by an external event (like a Switch).
  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "VA | Interact")
  bool bCanInteractDirectly = false;

  /** Select type for this object */
  UPROPERTY(EditDefaultsOnly, Category = "VA | Interaction")
  EVA_InteractionType InteractionAction = EVA_InteractionType::Interact;
#pragma endregion

#pragma region INTERNAL STATE
  bool bIsMoving = false;
  bool bIsAtTarget = false;
  float CurrentAlpha = 0.0f;

  FVector StartPos;
  FVector EndPos;
  FRotator StartRot;
  FRotator EndRot;
#pragma endregion

#pragma region INTERACTION
public:
  // Interface implementation
  virtual void OnInteract_Implementation(AActor* Interactor) override;

  // State getter. So that the Switch knows if it's moving or not.
  bool IsMoving() const { return bIsMoving; } 

	virtual FName GetInteractionRowName_Implementation() override;
#pragma endregion

#pragma region GETTER
  UFUNCTION(BlueprintGetter)
  bool GetCanInteractDirectly() const { return bCanInteractDirectly; }
#pragma endregion
};