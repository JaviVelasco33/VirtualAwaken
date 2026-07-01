// Fill out your copyright notice in the Description page of Project Settings.


#include "NPCs/VA_Companion.h"
#include "Character/VA_Character.h"
#include "Components/VA_AttributeComponent.h"
#include "Environment/VA_GasCloud.h"
#include "Projectiles/VA_BaseProjectile.h"
#include "Kismet/KismetMathLibrary.h"

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

	if (bIsReturningHome)
	{
		ExecuteFakeReturn(DeltaTime);
	}
	else if (bIsDistracting)
	{
		HandleDistractionMovement(DeltaTime);
	}
	else if (CurrentCombatTarget)
	{
		RotateTowardsTarget(DeltaTime);
	}
	else
	{
		HandleBasicFollow(DeltaTime);
	}
}
#pragma endregion

#pragma region ABILITIES
void AVA_Companion::ExecuteScan()
{
	if (bIsCompanionBusy || !GetWorld()) return;

	bIsCompanionBusy = true;

	// Set origin (himself)
	FVector ScanOrigin = GetActorLocation();

	// Trace config
	TArray<FHitResult> HitResults;
	FCollisionShape ScanSphere = FCollisionShape::MakeSphere(ScanRadius);

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);
	if (OwnerCharacter)
	{
		QueryParams.AddIgnoredActor(OwnerCharacter);
	}

	// execute "scan"
	bool bHit = GetWorld()->SweepMultiByChannel(
		HitResults,
		ScanOrigin,
		ScanOrigin,
		FQuat::Identity,
		ECC_Visibility,
		ScanSphere,
		QueryParams
	);

	// Process results
	if (bHit)
	{
		TSet<AActor*> ProcessedActors;

		for (const FHitResult& Hit : HitResults)
		{
			AActor* FoundActor = Hit.GetActor();
			if (FoundActor && FoundActor->ActorHasTag(TEXT("Scannable")) && !ProcessedActors.Contains(FoundActor))
			{
				ProcessedActors.Add(FoundActor);
				GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Orange, FString::Printf(TEXT("C0M-P4 has detected: %s"), *FoundActor->GetName()));
			}
		}
	}

	DrawDebugSphere(GetWorld(), ScanOrigin, ScanRadius, 32, FColor::Cyan, false, 1.0f, 0, 2.0f);
	bIsCompanionBusy = false;
}

void AVA_Companion::StartAssaultProtocol()
{
	if (bIsCompanionBusy || bIsAssaultOnCD || !OwnerCharacter || !ProjectileClass) return;

  // Clear any existing timers to avoid overlapping loops
	GetWorldTimerManager().ClearTimer(AssaultTH);
	GetWorldTimerManager().ClearTimer(DurationTH);

	CurrentCombatTarget = GetBestVisibleTarget();

	if (CurrentCombatTarget)
	{
		bIsCompanionBusy = true;
		bIsAssaultOnCD = true;
		// Start the assault loop
		AutomatedAssaultLoop();
		GetWorldTimerManager().SetTimer(AssaultTH, this, &AVA_Companion::AutomatedAssaultLoop, FireRate, true);
		bInAssaultMode = true;

		// Set a timer to stop the assault protocol after the specified duration
		GetWorldTimerManager().SetTimer(DurationTH, this, &AVA_Companion::StopAssaultProtocol, AssaultDuration, false);

		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, TEXT("C0M-P4: Assault protocol activated."));
	}
	else
	{
		// Abort instantly if no target is visible through walls
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Orange, TEXT("C0M-P4: Cannot engage. Target obstructed or out of range."));
	}
}

void AVA_Companion::StopAssaultProtocol()
{
	if (GetWorld())
	{
    // Clear the timer to stop the assault loop
		GetWorldTimerManager().ClearTimer(AssaultTH);
		GetWorldTimerManager().ClearTimer(DurationTH);
	}
  CurrentCombatTarget = nullptr;
	bInAssaultMode = false;
	bIsCompanionBusy = false;

	// Start Cooldown countdown
	GetWorldTimerManager().SetTimer(CooldownTH, this, &AVA_Companion::ResetAssaultCooldown, AbilitiesCD, false);

	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Cyan, TEXT("C0M-P4: Assault Protocol Finished."));
}

void AVA_Companion::StartGasProtocol()
{
	if (bIsCompanionBusy || bIsGasOnCD || !OwnerCharacter || !GasCloudClass) return;

	bIsGasOnCD = true;
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	// Get the player location
	FVector SpawnLoc = OwnerCharacter->GetActorLocation();

	// Adjust the height to be at the floor
	SpawnLoc.Z -= 90.f;

	// Spawn the BP_GasCloud
	GetWorld()->SpawnActor<AVA_GasCloud>(GasCloudClass, SpawnLoc, FRotator::ZeroRotator, SpawnParams);

	// Start Cooldown countdown
	GetWorldTimerManager().SetTimer(CooldownTH, this, &AVA_Companion::ResetGasCooldown, AbilitiesCD, false);


	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Cyan, TEXT("C0M-P4: Hiding protocol activated."));
}

void AVA_Companion::StartRepairProtocol()
{
	if (bIsCompanionBusy || bIsRepairOnCD || !OwnerCharacter || RemainingRepairTicks > 0) return;

	AttrComp = OwnerCharacter->FindComponentByClass<UVA_AttributeComponent>();

	if (AttrComp && AttrComp->GetCurrentHealth() < AttrComp->GetMaxHealth())
	{
		bIsRepairOnCD = true;
		bIsCompanionBusy = true;
		RemainingRepairTicks = 5;

		GetWorldTimerManager().SetTimer(RepairTH, this, &AVA_Companion::ApplyRepairTick, 1.0f, true);

		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, TEXT("C0M-P4: Starting repair..."));
	}
}

void AVA_Companion::CancelRepair()
{
	bIsRepairOnCD = false;
	bIsCompanionBusy = false;
	RemainingRepairTicks = 0;
	GetWorldTimerManager().ClearTimer(RepairTH);
	// Start Cooldown countdown
	GetWorldTimerManager().SetTimer(CooldownTH, this, &AVA_Companion::ResetRepairCooldown, AbilitiesCD, false);

}

void AVA_Companion::StartDistractionProtocol()
{
	if (bIsCompanionBusy || bIsDistractOnCD || !GetWorld() || !OwnerCharacter) return;

	CurrentDistractionTarget = GetBestVisibleTarget();

	if (CurrentDistractionTarget)
	{
		bIsCompanionBusy = true;
		bIsDistractOnCD = true;
		bIsDistracting = true;
		CurrentOrbitAngle = 0.0f;
		ReturnStage = 0;

		// Set auto shutdown
		GetWorldTimerManager().SetTimer(DistractionTH, this, &AVA_Companion::StopDistractionProtocol, DistractionDuration, false);

		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Magenta, TEXT("C0M-P4: Commencing Distraction Protocol. Drawing attention!"));
	}
}

void AVA_Companion::StopDistractionProtocol()
{
	bIsDistracting = false;
	CurrentDistractionTarget = nullptr;
	GetWorldTimerManager().ClearTimer(DistractionTH);

	bIsReturningHome = true;
	ReturnStage = 1;

	GetWorldTimerManager().SetTimer(CooldownTH, this, &AVA_Companion::ResetDistractCooldown, AbilitiesCD, false);

	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Magenta, TEXT("C0M-P4: Distraction finished. Returning to owner."));
}

void AVA_Companion::AutomatedAssaultLoop()
{
  CurrentCombatTarget = GetBestVisibleTarget();

	if (CurrentCombatTarget)
	{
    // Calculate ideal projectile launch trajectory
    FRotator SpawnRot = (CurrentCombatTarget->GetActorLocation() - GetActorLocation()).Rotation();

		// Spawn offset to prevent structural collision
    FVector SpawnLoc = GetActorLocation() + (SpawnRot.Vector() * 30.f);

    FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.Instigator = OwnerCharacter;
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		GetWorld()->SpawnActor<AVA_BaseProjectile>(ProjectileClass, SpawnLoc, SpawnRot, SpawnParams);

		GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, TEXT("C0M-P4: Firing automated burst!"));
	}
}

AActor* AVA_Companion::GetBestVisibleTarget()
{
  TArray<FHitResult> SweepResults;
  FCollisionShape Sphere = FCollisionShape::MakeSphere(ScanRadius);

	FCollisionQueryParams SweepParams;
	SweepParams.AddIgnoredActor(this);
	SweepParams.AddIgnoredActor(OwnerCharacter);

  // Broad Phase: Sweep for potential targets within ScanRadius
	bool bHit = GetWorld()->SweepMultiByChannel(
		SweepResults,
		GetActorLocation(),
		GetActorLocation(),
		FQuat::Identity,
		ECC_Visibility,
		Sphere,
		SweepParams
  );

  AActor* BestTarget = nullptr;
	float MinDistance = ScanRadius;

	if (bHit)
	{
		for (const FHitResult& SweepHit : SweepResults)
		{
			AActor* FoundActor = SweepHit.GetActor();
			if (FoundActor && FoundActor->ActorHasTag(TEXT("Enemy")))
			{
				FHitResult TraceHit;
        FVector TraceStart = GetActorLocation();
        FVector TraceEnd = FoundActor->GetActorLocation();

        FCollisionQueryParams TraceParams;
        TraceParams.AddIgnoredActor(this);
        TraceParams.AddIgnoredActor(OwnerCharacter);

        // Fire a precise beam straight to the target
				if (GetWorld()->LineTraceSingleByChannel(TraceHit, TraceStart, TraceEnd, ECC_Visibility, TraceParams))
				{
          // If the trace strikes something else (walls, obstacles, etc.), we ignore this target
					if (TraceHit.GetActor() != FoundActor)
					{
						continue;
					}
				}

				// Distance verification
				float DistanceToTarget = FVector::Dist(GetActorLocation(), FoundActor->GetActorLocation());
				if (DistanceToTarget < MinDistance)
				{
					MinDistance = DistanceToTarget;
					BestTarget = FoundActor;
				}
			}
		}
	}
  return BestTarget;
}

void AVA_Companion::RotateTowardsTarget(float DeltaTime)
{
	if (CurrentCombatTarget)
	{
    // Calculate look rotation towards the target
		FRotator TargetRot = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), CurrentCombatTarget->GetActorLocation());

    // Interpolate smoothly to the target rotation
		FRotator NewRot = FMath::RInterpTo(GetActorRotation(), TargetRot, DeltaTime, FollowSpeed);

    SetActorRotation(NewRot);
	}
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

void AVA_Companion::HandleDistractionMovement(float DeltaTime)
{
	if (!bIsDistracting || !CurrentDistractionTarget || !OwnerCharacter)	return;

	CurrentOrbitAngle += OrbitSpeed * DeltaTime;
	if (CurrentOrbitAngle > 360.0f)	CurrentOrbitAngle -= 360.0f;

  float RadianAngle = FMath::DegreesToRadians(CurrentOrbitAngle);
	float X = FMath::Cos(RadianAngle) * OrbitRadius;
	float Y = FMath::Sin(RadianAngle) * OrbitRadius;

	// Ideal position if there is no wall
	FVector EnemyLoc = CurrentDistractionTarget->GetActorLocation();
	FVector IdealOrbitLoc = EnemyLoc + FVector(X, Y, 90.f);

	// Launch a trace from the enemy center to the ideal orbit position
	FHitResult WallHit;
	FCollisionQueryParams TraceParams;
	TraceParams.AddIgnoredActor(this);
	TraceParams.AddIgnoredActor(CurrentDistractionTarget);
	TraceParams.AddIgnoredActor(OwnerCharacter);

	// Save the finel location modified by the walls
	FVector FinalTargetLoc = IdealOrbitLoc;

	// Search if there are walls in the orbit radius
	bool bHitWall = GetWorld()->LineTraceSingleByChannel(
		WallHit, 
		EnemyLoc + FVector(0, 0, 80.f), 
		IdealOrbitLoc, 
		ECC_WorldStatic, 
		TraceParams);

	if (bHitWall)
	{
		// If there is a wall, we shorten the orbit distance automatically
		FinalTargetLoc = WallHit.ImpactPoint + (WallHit.ImpactNormal * 20.f);

		DrawDebugLine(GetWorld(), EnemyLoc + FVector(0.f, 0.f, 80.f), WallHit.ImpactPoint, FColor::Red, false, 0.05f);
	}

	// Smoothly move towards the distraction spot
	FVector NewLoc = FMath::VInterpTo(GetActorLocation(), FinalTargetLoc, DeltaTime, FollowSpeed);
	SetActorLocation(NewLoc);

	// Keep drone facing the enemy while orbiting
	FRotator LookAtRot = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), CurrentDistractionTarget->GetActorLocation());
	SetActorRotation(FMath::RInterpTo(GetActorRotation(), LookAtRot, DeltaTime, FollowSpeed * 2.f));	
}

void AVA_Companion::ExecuteFakeReturn(float DeltaTime)
{
	if (!OwnerCharacter) return;

	if (ReturnStage == 1)
	{
		// Shoot straight up into the sky
    float TargetHeight = OwnerCharacter->GetActorLocation().Z + 600.f;
		FVector UpTarget = FVector(GetActorLocation().X, GetActorLocation().Y, TargetHeight);

    // Smoothly move upwards
		SetActorLocation(FMath::VInterpTo(GetActorLocation(), UpTarget, DeltaTime, FollowSpeed));

		// Once high enough teleport behing camera
		if (FMath::IsNearlyEqual(GetActorLocation().Z, UpTarget.Z, 10.f))
		{
			APlayerController* PC = Cast<APlayerController>(OwnerCharacter->GetController());

			if (PC)
			{
				FVector CamLoc;
				FRotator CamRot;
				PC->GetPlayerViewPoint(CamLoc, CamRot);

				// Teleport behind the player
				FVector BehindCamLoc = CamLoc - (CamRot.Vector() * 400.f);
				BehindCamLoc.Z = OwnerCharacter->GetActorLocation().Z + 90.f; // Keep it at the same height as the player
				SetActorLocation(BehindCamLoc);

				ReturnStage = 2;
			}
		}
	}
	else if (ReturnStage == 2)
	{
		// Smoothly blend back into the player
    FVector OffsetRotated = OwnerCharacter->GetActorRotation().RotateVector(OffsetFromOwner);
		FVector ShoulderTarget = OwnerCharacter->GetActorLocation() + OffsetRotated;

		SetActorLocation(FMath::VInterpTo(GetActorLocation(), ShoulderTarget, DeltaTime, FollowSpeed));

    FRotator NewRot = FMath::RInterpTo(GetActorRotation(), OwnerCharacter->GetActorRotation(), DeltaTime, FollowSpeed / 2.f);
		SetActorRotation(NewRot);

		if (FVector::Dist(GetActorLocation(), ShoulderTarget) < 30.f)
		{
			bIsReturningHome = false;
			ReturnStage = 0;
			bIsCompanionBusy = false;
			GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Cyan, TEXT("C0M-P4: Welcome home. Ready for next command."));
		}
	}
}

void AVA_Companion::HandleBasicFollow(float DeltaTime)
{
	if (!OwnerCharacter) return;

	// Save actual position
	FVector CurrentLoc = GetActorLocation();

	// Calculate the new position
	FVector OffsetRotated = OwnerCharacter->GetActorRotation().RotateVector(OffsetFromOwner);
	FVector IdealTargetLoc = OwnerCharacter->GetActorLocation() + OffsetRotated;

	// Safety Position
  FVector SafetyOffset = FVector(-75.f, 0.f, 90.f);
  FVector SafetyTargetLoc = OwnerCharacter->GetActorLocation() + SafetyOffset;

  FVector FinalTargetLoc = IdealTargetLoc;

  FHitResult ObstacleHit;
  FCollisionQueryParams TraceParams;
  TraceParams.AddIgnoredActor(this);
  TraceParams.AddIgnoredActor(OwnerCharacter);

  FVector TraceStart = OwnerCharacter->GetActorLocation() + FVector(0.f, 0.f, 90.f);

  FCollisionShape ComfortSphere = FCollisionShape::MakeSphere(45.f);

	bool bHitComfortZone = GetWorld()->SweepSingleByChannel(
		ObstacleHit,
		TraceStart,
		IdealTargetLoc,
    FQuat::Identity,
		ECC_WorldStatic,
		ComfortSphere,
    TraceParams
	);

	if (bHitComfortZone)
	{
		// If there is an obstacle, we adjust the target location to be in front of the obstacle
		FinalTargetLoc = SafetyTargetLoc;
    FHitResult EmergencyHit;

		bool bSafetyObstructed = GetWorld()->SweepSingleByChannel(
			EmergencyHit,
			TraceStart,
			SafetyTargetLoc,
			FQuat::Identity,
			ECC_WorldStatic,
			ComfortSphere,
			TraceParams
    );

		if (bSafetyObstructed)
		{
      FinalTargetLoc = EmergencyHit.ImpactPoint + (EmergencyHit.ImpactNormal * 20.f);
		}
	}

	// Move with smooth interp
	FVector NewLoc = FMath::VInterpTo(CurrentLoc, FinalTargetLoc, DeltaTime, FollowSpeed);
	SetActorLocation(NewLoc);

	// Save actual rotation
	FRotator CurrentRot = GetActorRotation();
	FRotator TargetRot;

	// Calculate vector direction
	FVector MoveDir = FinalTargetLoc - CurrentLoc;

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

	if (bInAssaultMode)
	{
		RotateTowardsTarget(DeltaTime);
	}
	else
	{
		// Apply smooth rotation
		SetActorRotation(FMath::RInterpTo(CurrentRot, TargetRot, DeltaTime, FollowSpeed / 2.f));
	}
}

#pragma endregion