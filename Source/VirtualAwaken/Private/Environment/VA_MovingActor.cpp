// Fill out your copyright notice in the Description page of Project Settings.


#include "Environment/VA_MovingActor.h"
#include "Characters/VA_Character.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"

#pragma region INIT
// Sets default values
AVA_MovingActor::AVA_MovingActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
  PrimaryActorTick.bStartWithTickEnabled = false;

  RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("RootComp"));
	RootComponent = RootComp;

  PivotComp = CreateDefaultSubobject<USceneComponent>(TEXT("PivotComp"));
  PivotComp->SetupAttachment(RootComp);

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
  MeshComp->SetupAttachment(PivotComp);
}

// Called when the game starts or when spawned
void AVA_MovingActor::BeginPlay()
{
	Super::BeginPlay();

  // Save initial position/rotation as Point A
  StartPos = PivotComp->GetRelativeLocation();
  StartRot = PivotComp->GetRelativeRotation();

  // Calculate the final destination (Point B)
  if (MovementType == EVA_MovementType::Translation)
  {
    EndPos = StartPos + FinalPosition;
  }
  else
  {
    // In rotation, use X=Roll, Y=Pitch and Z=Yaw from vector FinalPosition.
    EndRot = StartRot + FRotator(FinalPosition.Y, FinalPosition.Z, FinalPosition.X);
  }
	
}

// Called every frame
void AVA_MovingActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

  // Calculate alpha with easing curve
  float TargetAlpha = bIsAtTarget ? 0.0f : 1.0f;

  // Constant interpolate of time (Alpha)
  CurrentAlpha = FMath::FInterpConstantTo(CurrentAlpha, TargetAlpha, DeltaTime, MoveSpeed * 0.5f);

  // Apply easing curve to alpha
  float SmoothAlpha = FMath::InterpEaseInOut(0.0f, 1.0f, CurrentAlpha, EaseAmount);

  // Apply transformation based on movement type
  if (MovementType == EVA_MovementType::Translation)
  {
    PivotComp->SetRelativeLocation(FMath::Lerp(StartPos, EndPos, SmoothAlpha));
  }
  else
  {
    PivotComp->SetRelativeRotation(FMath::Lerp(StartRot, EndRot, SmoothAlpha));
  }

  // Check if we reached the destination
  if (FMath::IsNearlyEqual(CurrentAlpha, TargetAlpha, 0.001f))
  {
    CurrentAlpha = TargetAlpha;
    bIsAtTarget = !bIsAtTarget; // Switch target for next time
    bIsMoving = false;
    SetActorTickEnabled(false);
  }
}



#pragma endregion

#pragma region INTERACTION
void AVA_MovingActor::OnInteract_Implementation(AActor* Interactor)
{
  if (bIsMoving) return;

  if (!bCanInteractDirectly && Interactor && Interactor->IsA<AVA_Character>()) return;

  bIsMoving = true;
  SetActorTickEnabled(true);
}

FName AVA_MovingActor::GetInteractionRowName_Implementation()
{
  switch (InteractionAction)
  {
    case EVA_InteractionType::Interact:
      return FName("Interact");
      break;
    case EVA_InteractionType::Open:
      return FName("Open");
      break;
    case EVA_InteractionType::Talk:
      return FName("Talk");
      break;
    case EVA_InteractionType::Press:
      return FName("Press");
      break;
    case EVA_InteractionType::Grab:
      return FName("Grab");
      break;
    default:
      return FName("Interact");
      break;
  }
}

#pragma endregion