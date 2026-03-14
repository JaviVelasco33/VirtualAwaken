// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "Core/VA_InteractionSettings.h"
#include "VA_Character.generated.h"

UCLASS()
class VIRTUALAWAKEN_API AVA_Character : public ACharacter
{
	GENERATED_BODY()

#pragma region INIT
public:
	// Sets default values for this character's properties
	AVA_Character();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
#pragma endregion

#pragma region COMPONENTS
protected:
	// Spring arm to control the distance and collision
  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "VA | Camera")
	TObjectPtr<class USpringArmComponent> CameraBoom;

	// Cam that follows the player
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "VA | Camera")
	TObjectPtr<class UCameraComponent> FollowCamera;

	// 
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "VA | Components")
	TObjectPtr<class UVA_AttributeComponent> AttributeComponent;

  // Interaction component to handle interactions with the environment
  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "VA | Components")
  TObjectPtr<class UVA_InteractionComponent> InteractionComponent;
#pragma endregion

#pragma region GETTERS
	// Getters for components
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
#pragma endregion

#pragma region INPUT ASSETS
  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "VA | Input")
	TObjectPtr<class UInputMappingContext> DefaultMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "VA | Input")
	TObjectPtr<class UInputAction> MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "VA | Input")
	TObjectPtr<class UInputAction> LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "VA | Input")
	TObjectPtr<class UInputAction> JumpAction;

	// Interact Action
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "VA | Input")
	TObjectPtr<class UInputAction> InteractAction;
#pragma endregion

#pragma region INPUT CALLBACKS
	// Call for movement input
	void Move(const FInputActionValue& Value);

	// Call for mouse or right stick input
	void Look(const FInputActionValue& Value);

	void StartJump();
	void StopJump();
#pragma endregion

#pragma region INTERACTION SETTINGS
		// Function called by the Input
		void Interact();

    bool bIsRotatingToInteract = false;
    FRotator TargetInteractRotation;
#pragma endregion

#pragma region ATTRIBUTES

#pragma endregion
};
