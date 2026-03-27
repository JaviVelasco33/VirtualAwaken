// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/VA_Companion.h"
#include "Characters/VA_Character.h"
#include "Components/VA_AttributeComponent.h"
#include "Environment/VA_GasCloud.h"

#pragma region INIT
// Sets default values
AVA_Companion::AVA_Companion()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	SphereComponent->InitSphereRadius(60.f);
	SphereComponent->SetCollisionProfileName(UCollisionProfile::Pawn_ProfileName);
	SphereComponent->CanCharacterStepUpOn = ECB_No;
	SphereComponent->SetShouldUpdatePhysicsVolume(true);
	SphereComponent->SetCanEverAffectNavigation(false);
	SphereComponent->bDynamicObstacle = true;
	RootComponent = SphereComponent;

  Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->AlwaysLoadOnClient = true;
	Mesh->AlwaysLoadOnServer = true;
	Mesh->bOwnerNoSee = false;
	Mesh->bCastDynamicShadow = true;
	Mesh->bAffectDynamicIndirectLighting = true;
	Mesh->PrimaryComponentTick.TickGroup = TG_PrePhysics;
	Mesh->SetupAttachment(SphereComponent);
	static FName MeshCollisionProfileName(TEXT("CharacterMesh"));
	Mesh->SetCollisionProfileName(MeshCollisionProfileName);
	Mesh->SetGenerateOverlapEvents(false);
	Mesh->SetCanEverAffectNavigation(false);

}

// Called when the game starts or when spawned
void AVA_Companion::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AVA_Companion::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (OwnerCharacter)
	{
		// Save actual position
		FVector CurrentLoc = GetActorLocation();

		// Calculate the new position
		FVector OffsetRotated = OwnerCharacter->GetActorRotation().RotateVector(OffsetFromOwner);
		FVector TargetLoc = OwnerCharacter->GetActorLocation() + OffsetRotated;

		// Move with smooth interp
		FVector NewLoc = FMath::VInterpTo(CurrentLoc, TargetLoc, DeltaTime, FollowSpeed);
		SetActorLocation(NewLoc);

		// Save actual rotation
		FRotator CurrentRot = GetActorRotation();
		FRotator TargetRot;

		// Calculate vector direction
		FVector MoveDir = TargetLoc - CurrentLoc;

		// Check if it is moving (with tolerance)
		if (MoveDir.SizeSquared() > 50000.f)
		{
			// Look forward direction
			TargetRot = MoveDir.Rotation();
		}
		else
		{
			// If has arrive his position, copy player rotation
			TargetRot = OwnerCharacter->GetActorRotation();
		}
		// Apply smooth rotation
		SetActorRotation(FMath::RInterpTo(CurrentRot, TargetRot, DeltaTime, FollowSpeed / 2.f));
	}

}
#pragma endregion

#pragma region ABILITIES
void AVA_Companion::ExecuteScan()
{
	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Cyan, TEXT("C0M-P4: Scanning area..."));
}

void AVA_Companion::StartGasProtocol()
{
	if (!OwnerCharacter || !GasCloudClass) return;

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	// Get the player location
	FVector SpawnLoc = OwnerCharacter->GetActorLocation();

	// Adjust the height to be at the floor
	SpawnLoc.Z -= 90.f;

	// Spawn the BP_GasCloud
	GetWorld()->SpawnActor<AVA_GasCloud>(GasCloudClass, SpawnLoc, FRotator::ZeroRotator, SpawnParams);

	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Cyan, TEXT("C0M-P4: Hiding protocol activated."));
}

void AVA_Companion::StartRepairProtocol()
{
	if (!OwnerCharacter || RemainingRepairTicks > 0) return;

	AttrComp = OwnerCharacter->FindComponentByClass<UVA_AttributeComponent>();

	if (AttrComp && AttrComp->GetCurrentHealth() < AttrComp->GetMaxHealth())
	{
		RemainingRepairTicks = 5;

		GetWorldTimerManager().SetTimer(RepairTH, this, &AVA_Companion::ApplyRepairTick, 1.0f, true);

		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, TEXT("C0M-P4: Starting repair..."));
	}
}

void AVA_Companion::CancelRepair()
{
	RemainingRepairTicks = 0;
	GetWorldTimerManager().ClearTimer(RepairTH);
}

void AVA_Companion::ApplyRepairTick()
{
	if (OwnerCharacter && RemainingRepairTicks > 0)
	{
		AttrComp = OwnerCharacter->FindComponentByClass<UVA_AttributeComponent>();
		if (AttrComp)
		{
			// Add health to player component
			AttrComp->ApplyHealthChange(HealthPerTick, this);

			RemainingRepairTicks--;

			// If max health is reach or the ticks is ended, stop it
			if (RemainingRepairTicks <= 0 || AttrComp->GetCurrentHealth() >= AttrComp->GetMaxHealth())
			{
				CancelRepair();
				GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, TEXT("C0M-P4: Repair completed."));
			}
		}
	}
	else
	{
		CancelRepair();
	}
}

#pragma endregion