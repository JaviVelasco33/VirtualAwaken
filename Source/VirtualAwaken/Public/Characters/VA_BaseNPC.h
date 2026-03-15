// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/VA_InteractableInterface.h"
#include "Core/Types/VA_NPCType.h"
#include "Core/Dialogues/VA_DialogueAsset.h"
#include "VA_BaseNPC.generated.h"

UCLASS()
class VIRTUALAWAKEN_API AVA_BaseNPC : public ACharacter, public IVA_InteractableInterface
{
	GENERATED_BODY()

#pragma region INIT
public:
	// Sets default values for this character's properties
	AVA_BaseNPC();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
#pragma endregion

#pragma region INTERACTION
protected:
	// Interaction config
  UPROPERTY(EditDefaultsOnly, Category = "VA | Interaction")
	FName InteractionRowName = FName("Talk");

#pragma endregion

#pragma region DIALOGUE
protected:
	// Dialogs system
  UPROPERTY(EditAnywhere, Category = "VA | Dialogue")
	UVA_DialogueAsset* DialogueAsset;

	// Array of shorts phrases that the NPC says when he don't want to talk
  UPROPERTY(EditAnywhere, Category = "VA | Dialogue")
  TArray<FText> AmbientBarks;

  int32 CurrentDialogueIndex = 0;

	UFUNCTION()
  void PlayAmbientBark();
#pragma endregion

#pragma region CONFIG
  UPROPERTY(EditDefaultsOnly, Category = "VA | Config")
  EVA_NPCType NPCType = EVA_NPCType::Neutral;
#pragma endregion
#pragma region INTERFACE
public:
  // Implement the interaction interface
	virtual FName GetInteractionRowName_Implementation() override;
  virtual void OnInteract_Implementation(AActor* Interactor) override;

	// Function to advance the dialog
	void ShowNextLine();

	bool bIsRotatingToInteract = false;
	FRotator TargetInteractRotation;
#pragma endregion
};
