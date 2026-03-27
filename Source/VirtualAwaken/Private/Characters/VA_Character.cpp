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
#include "Core/Dialogues/VA_DialogueManager.h"
#include "Characters/VA_Companion.h"
#include "Kismet/GameplayStatics.h"

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
	GetCharacterMovement()->MaxWalkSpeed = 500.f;

	// Create the SpringArm
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 350.f;
	CameraBoom->TargetOffset = FVector(0.f, 0.f, 70.f);
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

	WalkSpeed = GetCharacterMovement()->MaxWalkSpeed;
	SprintSpeed = WalkSpeed * 1.8f;

	PlayerController = Cast<APlayerController>(Controller);
	
	// Add the Mapping context on init
	if (PlayerController)
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
		if (PlayerController)
		{
			FRotator CurrentRot = PlayerController->GetControlRotation();

			float NewYaw = CurrentRot.Yaw + (10.f * DeltaTime);
			float TargetPitch = -25.f;

			FRotator TargetRot = FRotator(TargetPitch, CurrentRot.Yaw, 0.f);
			FRotator NewRot = FMath::RInterpTo(CurrentRot, TargetRot, DeltaTime, 0.75f);

			NewRot.Yaw = NewYaw;

			PlayerController->SetControlRotation(NewRot);
		}

		float CurrentArm = CameraBoom->TargetArmLength;
		float TargetArm = 600.f;

		CameraBoom->TargetArmLength = FMath::FInterpTo(CurrentArm, TargetArm, DeltaTime, 2.0f);
	}
}

// Called to bind functionality to input
void AVA_Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
  Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Cast the input component to Enhanced system
	if (UEnhancedInputComponent* EIC = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Link the move actions
		EIC->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AVA_Character::Move);
		EIC->BindAction(MoveAction, ETriggerEvent::Completed, this, &AVA_Character::StopSprintOnMoveEnd);

		// Link the look actions
		EIC->BindAction(LookAction, ETriggerEvent::Triggered, this, &AVA_Character::Look);

		// Link the jump action
		EIC->BindAction(JumpAction, ETriggerEvent::Started, this, &AVA_Character::StartJump);
		EIC->BindAction(JumpAction, ETriggerEvent::Completed, this, &AVA_Character::StopJump);

		// Link the interact action
		EIC->BindAction(InteractAction, ETriggerEvent::Started, this, &AVA_Character::Interact);

		// Link the attack action
		EIC->BindAction(AttackAction, ETriggerEvent::Started, this, &AVA_Character::Attack);

		// Link the dash action
		EIC->BindAction(DashAction, ETriggerEvent::Started, this, &AVA_Character::Dash);

		// Link the scanner action
		EIC->BindAction(ScannerAction, ETriggerEvent::Started, this, &AVA_Character::Scanner);

		// Link the gadget action
		EIC->BindAction(GadgetAction, ETriggerEvent::Started, this, &AVA_Character::Gadget);

		// Link the aim action
		EIC->BindAction(AimAction, ETriggerEvent::Started, this, &AVA_Character::AimStart);
		EIC->BindAction(AimAction, ETriggerEvent::Completed, this, &AVA_Character::AimEnd);

		// Link the shoot action
		EIC->BindAction(ShootAction, ETriggerEvent::Started, this, &AVA_Character::Shoot);

		// Link the lock on action
		EIC->BindAction(LockOnAction, ETriggerEvent::Started, this, &AVA_Character::LockOn);

		// Link the sprint action
		EIC->BindAction(SprintAction, ETriggerEvent::Started, this, &AVA_Character::ToggleSprint);

		// Link the orders action
		EIC->BindAction(Order1Action, ETriggerEvent::Started, this, &AVA_Character::CompanionOrders);
		EIC->BindAction(Order2Action, ETriggerEvent::Started, this, &AVA_Character::CompanionOrders);
		EIC->BindAction(Order3Action, ETriggerEvent::Started, this, &AVA_Character::CompanionOrders);
		EIC->BindAction(Order4Action, ETriggerEvent::Started, this, &AVA_Character::CompanionOrders);

		// Link the map action
		EIC->BindAction(MapAction, ETriggerEvent::Started, this, &AVA_Character::ShowMap);

		// Link the pause action
		EIC->BindAction(PauseAction, ETriggerEvent::Started, this, &AVA_Character::PauseGame);
	}
}
#pragma endregion

#pragma region INPUT CALLBACKS
void AVA_Character::Move(const FInputActionValue& Value)
{
	if (bInDialogueMode) return;

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

//When stop moving
void AVA_Character::StopSprintOnMoveEnd()
{
	if (bInDialogueMode) return;

	if (GetCharacterMovement())
	{
		GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	}
}

void AVA_Character::Look(const FInputActionValue& Value)
{
	if (bInDialogueMode) return;

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
	if (bInDialogueMode) return;

	Jump();
}

void AVA_Character::StopJump()
{
	if (bInDialogueMode) return;

	StopJumping();
}

void AVA_Character::Attack()
{
	if (bInDialogueMode) return;

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Attack")));
	}
}

void AVA_Character::Dash()
{
	if (bInDialogueMode) return;

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Dash")));
	}
}

void AVA_Character::Scanner()
{
	if (bInDialogueMode) return;

	if (ActiveCompanion)
	{
		ActiveCompanion->ExecuteScan();
	}
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Scanner")));
	}
}

void AVA_Character::Gadget()
{
	if (bInDialogueMode) return;

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Gadget")));
	}
}

void AVA_Character::AimStart()
{
	if (bInDialogueMode) return;

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("AimStart")));
	}
}

void AVA_Character::AimEnd()
{
	if (bInDialogueMode) return;

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("AimEnd")));
	}
}

void AVA_Character::Shoot()
{
	if (bInDialogueMode) return;

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Shoot")));
	}
}

void AVA_Character::LockOn()
{
	if (bInDialogueMode) return;

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("LockOn")));
	}
}

void AVA_Character::ToggleSprint()
{
	if (bInDialogueMode) return;

	if (!GetCharacterMovement()) return;

	if (GetCharacterMovement()->MaxWalkSpeed <= WalkSpeed)
	{
		GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
	}
	else
	{
		GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	}
}

void AVA_Character::CompanionOrders(const FInputActionInstance& Instance)
{
	if (bInDialogueMode) return;

	// Get the action that was just pressed
	const UInputAction* TriggeredAction = Instance.GetSourceAction();

	if (TriggeredAction == Order1Action)
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("CompanionOrder_1")));
		}
	}

	if (TriggeredAction == Order2Action)
	{
		if (ActiveCompanion)
		{
			ActiveCompanion->StartGasProtocol();
			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("CompanionOrder_2")));
			}
		}
	}

	if (TriggeredAction == Order3Action)
	{
		if (ActiveCompanion)
		{
			ActiveCompanion->StartRepairProtocol();
      if (GEngine)
      {
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("CompanionOrder_3")));
      }
		}
	}

	if (TriggeredAction == Order4Action)
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("CompanionOrder_4")));
		}
	}
}

void AVA_Character::ShowMap()
{
	if (bInDialogueMode) return;

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("ShowMap")));
	}
}

void AVA_Character::PauseGame()
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("PauseGame")));
	}
}

#pragma endregion

#pragma region INTERACTION

void AVA_Character::Interact()
{
	UVA_DialogueManager* DM = GetGameInstance()->GetSubsystem<UVA_DialogueManager>();
	if (DM && DM->IsDialogueActive())
	{
		DM->NextStep();
		return;
	}

	if (InteractionComponent)
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
}
#pragma endregion

#pragma region DIALOGUE
void AVA_Character::SetDialogueCameraMode(bool bActive)
{
	bInDialogueMode = bActive;

	if (bActive)
	{
		OriginalArmLength = CameraBoom->TargetArmLength;
		OriginalRotation = PlayerController->GetControlRotation();
	}
	else
	{
		CameraBoom->TargetArmLength = OriginalArmLength;
		PlayerController->SetControlRotation(OriginalRotation);
	}
}
#pragma endregion

#pragma region SNEAK
void AVA_Character::SetInsideHiddingZone(bool bEntered)
{
	if (bEntered) HidingZonesCount++;
	else HidingZonesCount--;

	HidingZonesCount = FMath::Max(0, HidingZonesCount);

	bool bWasHidden = bIsHidden;
	bIsHidden = (HidingZonesCount > 0);

	if (bWasHidden != bIsHidden)
	{
		//OnSneakStateChanged(bIsHidden);
	}

	if (bEntered)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Hide")));
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Show")));
	}
}
#pragma endregion

#pragma region COMPANION
void AVA_Character::AttachCompanion()
{
	TArray<AActor*> FoundCompanions;

	// Search all actors of class Companion at the level
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AVA_Companion::StaticClass(), FoundCompanions);

	// If there is one at least, link it
	if (FoundCompanions.Num() > 0)
	{
		ActiveCompanion = Cast<AVA_Companion>(FoundCompanions[0]);

		if (ActiveCompanion)
		{
			ActiveCompanion->OwnerCharacter = this;
			ActiveCompanion->SetActorHiddenInGame(false);
			ActiveCompanion->SetActorTickEnabled(true);

			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("C0M-P4 added and systems activated."));
		}
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Error: Doesn't found any C0M-P4 in this level."));
	}
}
#pragma endregion