// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/VA_InteractableInterface.h"
#include "VA_NPCActor.generated.h"

UCLASS()
class VIRTUALAWAKEN_API AVA_NPCActor : public AActor, public IVA_InteractableInterface
{
	GENERATED_BODY()
	
#pragma region INIT
public:	
	// Sets default values for this actor's properties
	AVA_NPCActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
#pragma endregion

#pragma region COMPONENTS
protected:
  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "VA | Components")
	TObjectPtr<USceneComponent> RootComp;

  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "VA | Components")
  TObjectPtr<UStaticMeshComponent> MeshComp;
#pragma endregion

#pragma region IDENTITY
  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "VA | Identity")
  FName NPCName;
#pragma endregion

#pragma region FUNCTIONS
public:
  // Implement Interactable Interface
  virtual void OnInteract_Implementation(AActor* Interactor) override;

  // Function that child will override to show his own dialogue
  UFUNCTION(BlueprintNativeEvent, Category = "VA | NPC")
  void ShowDialogue();
#pragma endregion

};
