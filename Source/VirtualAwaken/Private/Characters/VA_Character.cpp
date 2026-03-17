// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/VA_Character.h"
#include "Camera/CameraComponent.h"
#include "Components/VA_AttributeComponent.h"
#include "Components/VA_InteractionComponent.h"
#include "Engine/OverlapResult.h"
#include "EnhancedInput/Public/EnhancedInputSubsystems.h"
#include "EnhancedInput/Public/EnhancedInputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Interfaces/VA_InteractableInterface.h"
#include "Kismet/KismetSystemLibrary.h"

#pragma region CONSTRUCTOR
// Sets default values
AVA_Character::AVA_Character()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// ------ Camera config ------
	// Config rotation for 3rd person
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Character rotate to movement direction
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 500.f, 0.f);

	// Create the SpringArm
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.f;
	CameraBoom->bUsePawnControlRotation = true;
	CameraBoom->bEnableCameraLag = true;
	CameraBoom->bEnableCameraRotationLag = true;
	CameraBoom->CameraLagSpeed = 15.0f;
	CameraBoom->CameraRotationLagSpeed = 15.0f;

	// Create the Camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	// ------ Movement config (Platform) ------
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 500.f, 0.f);

	// Jump config
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->GravityScale = 1.75f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.f;

	// ------ Attribute Component ------
	AttributeComponent = CreateDefaultSubobject<UVA_AttributeComponent>(TEXT("AttributeAcomponent"));

	// ------ Interaction Component ------
	InteractionComponent = CreateDefaultSubobject<UVA_InteractionComponent>(TEXT("InteractionComponent"));
}
#pragma endregion

#pragma region INIT
// Called when the game starts or when spawned
void AVA_Character::BeginPlay()
{
	Super::BeginPlay();
	
	// Add the Mapping context on init
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem <UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

// Called every frame
void AVA_Character::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Rotation when interact
	if (bIsRotatingToInteract)
	{
		FRotator CurrentRot = GetActorRotation();
    FRotator SmoothRot = FMath::RInterpTo(CurrentRot, TargetInteractRotation, DeltaTime, 5.f);
    SetActorRotation(SmoothRot);

    if (CurrentRot.Equals(TargetInteractRotation, 5.f))
    {
      bIsRotatingToInteract = false;
    }
	}

	// Camera orbit for dialogues
	if (bInDialogueMode)
	{
		// Add a bit rotation for the orbit and adjust Pitch to look from a bit higher
		APlayerController* PC = Cast<APlayerController>(GetController());
		if (PC)
		{
			FRotator CurrentRot = PC->GetControlRotation();

			float NewYaw = CurrentRot.Yaw + (10.f * DeltaTime);
			float TargetPitch = -25.f;

			FRotator TargetRot = FRotator(TargetPitch, CurrentRot.Yaw, 0.f);
			FRotator NewRot = FMath::RInterpTo(CurrentRot, TargetRot, DeltaTime, 1.5f);

			NewRot.Yaw = NewYaw;

			PC->SetControlRotation(NewRot);
		}
	}
}

// Called to bind functionality to input
void AVA_Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
  Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Cast the input component to Enhanced system
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Link the move and look actions
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AVA_Character::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AVA_Character::Look);

		// Link the jump action
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &AVA_Character::StartJump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &AVA_Character::StopJump);

		// Link the interact action
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Started, this, &AVA_Character::Interact);
	}
}
#pragma endregion

#pragma region MOVEMENT
void AVA_Character::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// Search the camera direction
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// Get forward direction
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		// Get side direction
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// Apply movement
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void AVA_Character::Look(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// Add rotation input to controller (Pitch and Yaw)
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void AVA_Character::StartJump()
{
	Jump();
}

void AVA_Character::StopJump()
{
	StopJumping();
}
#pragma endregion

#pragma region INTERACTION

void AVA_Character::Interact()
{
	AActor* Candidate = InteractionComponent->GetBestCandidate();
	if (Candidate)
	{
    FVector Direction = Candidate->GetActorLocation() - GetActorLocation();
		Direction.Z = 0.f;
    TargetInteractRotation = Direction.Rotation();
    bIsRotatingToInteract = true;
	}

  InteractionComponent->PrimaryInteract();
}

void AVA_Character::SetDialogueCameraMode(bool bActive)
{
	bInDialogueMode = bActive;

	if (bActive)
	{
		OriginalArmLength = CameraBoom->TargetArmLength;
		OriginalRotation = GetControlRotation();

		CameraBoom->TargetArmLength = 600.f;
	}
	else
	{
		CameraBoom->TargetArmLength = OriginalArmLength;
	}
}

#pragma endregion