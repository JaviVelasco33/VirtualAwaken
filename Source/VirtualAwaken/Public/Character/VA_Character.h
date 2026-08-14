// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "Core/VA_InteractionSettings.h"
#include "Engine/TimerHandle.h"
#include "Core/Types/VA_V311Phase.h"
#include "VA_Character.generated.h"

class AVA_Companion;

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

	// Component to manage the health and energy
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "VA | Components")
	TObjectPtr<class UVA_AttributeComponent> AttributeComponent;

  // Interaction component to handle interactions with the environment
  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "VA | Components")
  TObjectPtr<class UVA_InteractionComponent> InteractionComponent;

	// Collision in the right hand to apply damage to the enemies
  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "VA | Components")
  TObjectPtr<class UBoxComponent> AttackCollisionComponent;
#pragma endregion

#pragma region GETTERS
	// Getters for components
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
	FORCEINLINE class AVA_Companion* GetCompanion() const { return ActiveCompanion; }

#pragma endregion

#pragma region INPUT ASSETS
	// Context
  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "VA | Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputMappingContext> DefaultMappingContext;

	//Movement
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "VA | Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> MoveAction;

	// Camera
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "VA | Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> LookAction;

	// Jump
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "VA | Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> JumpAction;

	// Interaction
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "VA | Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> InteractAction;

	//Attack
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "VA | Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> AttackAction;

	// Dash
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "VA | Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> DashAction;

	// Scanner
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "VA | Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> ScannerAction;

	// Gadget
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "VA | Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> GadgetAction;

	//Aim
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "VA | Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> AimAction;

	// Shoot
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "VA | Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> ShootAction;

	// Lock On
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "VA | Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> LockOnAction;

	// Sprint
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "VA | Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> SprintAction;

	// Orders
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "VA | Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> Order1Action;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "VA | Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> Order2Action;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "VA | Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> Order3Action;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "VA | Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> Order4Action;

	// Map
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "VA | Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> MapAction;

	// Pause
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "VA | Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> PauseAction;
#pragma endregion

#pragma region INPUT CALLBACKS
	// Call for movement input
	void Move(const FInputActionValue& Value);
	void StopSprintOnMoveEnd();

	// Call for mouse or right stick input
	void Look(const FInputActionValue& Value);
	void StartJump();
	void StopJump();
	void Attack();
	void Dash();
	void Scanner();
	void Gadget();
	void AimStart();
	void AimEnd();
	void Shoot();
	void ToggleLockOn();
	void ToggleSprint();
	void CompanionOrders(const struct FInputActionInstance& Instance);
	void ShowMap();
	void PauseGame();

#pragma endregion

#pragma region INTERACTION
		// Function called by the Input
		void Interact();

    bool bIsRotatingToInteract = false;
    FRotator TargetInteractRotation;
#pragma endregion

#pragma region ATTRIBUTES
public:
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VA | Phases")
		EVA_Phase CurrentPhase = EVA_Phase::Phase1;

		UFUNCTION(BlueprintCallable)
		void CheckPhases();

		UFUNCTION(BlueprintCallable)
		void SetupAttributes();

protected:
	float SpeedMultiplier = 1.0f;

#pragma endregion

#pragma region DIALOGUE
public:
	void SetDialogueCameraMode(bool bActive);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "VA | Camera")
	bool bInDialogueMode = false;

	float OriginalArmLength;
	FRotator OriginalRotation;

private:
	UPROPERTY()
	APlayerController* PlayerController;

	// Movement Speed
	float WalkSpeed;
	float SprintSpeed;

#pragma endregion

#pragma region SNEAK
public:
	// Function to zones advise the player
	UFUNCTION(BlueprintCallable)
	void SetInsideHiddingZone(bool bEntered);
	
	// How many sneaking zones are covering right now
	int32 HidingZonesCount = 0;

	UPROPERTY(BlueprintReadOnly, Category = "VA | Sneak")
	bool bIsHidden = false;
#pragma endregion

#pragma region COMPANION
	// Pointer that will save the companion
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "VA | Companion")
	TObjectPtr<AVA_Companion> ActiveCompanion;

	// Search and attach companion
	UFUNCTION(BlueprintCallable, Category = "VA | Companion")
	void AttachCompanion();
#pragma endregion

#pragma region DASH
public:
	// Dash Cooldown
	UPROPERTY(EditAnywhere, Category = "VA | Dash")
	float DashCD = 1.5f;

	// Force apply to dash
	UPROPERTY(EditAnywhere, Category = "VA | Dash")
	float DashForce = 2500.f;
protected:
	UPROPERTY(EditDefaultsOnly, Category = "VA | Dash")
	UAnimMontage* DashAnim;

private:
	FTimerHandle DashTH;
	// Function to block multiple uses of dash until it reset
	void ApplyDashCD();
	bool bCanDash = true;

#pragma endregion

#pragma region LOCK ON/OFF
public:

protected:

private:
	bool bShouldLockOn = true;

#pragma endregion

#pragma region SPRINT
public:

protected:

private:
	bool bIsSprinting = false;

#pragma endregion

#pragma region ATTACK
public:
	UFUNCTION()
	void OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	// Toggle Attack collision
  UFUNCTION(BlueprintCallable, Category = "Combat")
  void SetAttackCollisionEnabled(bool bEnable);

protected:
	UPROPERTY(EditDefaultsOnly, Category = "VA | Attack")
	UAnimMontage* AttackAnim;

	UPROPERTY(EditAnywhere, Category = "VA | Attack")
	float AttackDamage = 10.f;


private:

#pragma endregion
#pragma region 
public:

protected:

private:

#pragma endregion
};
