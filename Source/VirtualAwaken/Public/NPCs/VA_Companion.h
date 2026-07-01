// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "VA_Companion.generated.h"

class AVA_Character;
class AVA_GasCloud;
class AVA_BaseProjectile;

UCLASS()
class VIRTUALAWAKEN_API AVA_Companion : public AActor
{
	GENERATED_BODY()
	
#pragma region INIT
public:	
	// Sets default values for this actor's properties
	AVA_Companion();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
#pragma endregion

#pragma region COMPONENTS
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "VA | Companion", meta=(AllowPrivateAccess = "true"))
	TObjectPtr<class USphereComponent> SphereComponent;

	/** The main skeletal mesh associated with this Character (optional sub-object). */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "VA | Companion", meta=(AllowPrivateAccess = "true"))
	TObjectPtr<class UStaticMeshComponent> Mesh;
#pragma endregion

#pragma region CHARACTER
public:
	// Owner reference
	UPROPERTY(BlueprintReadWrite, Category = "VA | Companion")
	TObjectPtr<class AVA_Character> OwnerCharacter;

	// Floating distance
	UPROPERTY(EditAnywhere, Category = "VA | Companion")
	FVector OffsetFromOwner = FVector(0.f, -75.f, 90.f);

	// Follow speed
	UPROPERTY(EditAnywhere, Category = "VA | Companion")
	float	FollowSpeed = 5.f;

	// Attribute componente (Player)
	UPROPERTY(BlueprintReadWrite, Category = "VA | Companion")
	TObjectPtr<class UVA_AttributeComponent> AttrComp;
#pragma endregion

#pragma region ABILITIES
public:
	// Activate Scan
	void ExecuteScan();

  // Order 1 - Start attacking
  void StartAssaultProtocol();

  // Order 1 - Stop attacking
	void StopAssaultProtocol();

	// Order 2 - Start hiding
	void StartGasProtocol();

	// Order 3 - Start healing
	void StartRepairProtocol();

	// Order 3 - Cancel healing
	void CancelRepair();

	// Order 4 - Start Distraction
	void StartDistractionProtocol();

	// Order 4- Stop Distraction
	void StopDistractionProtocol();

protected:
	UPROPERTY(EditAnywhere, Category = "VA | Abilities")
    float AbilitiesCD = 10.0f;
#pragma region SCANNER
protected:
	// Scan wave radius
	UPROPERTY(EditAnywhere, Category = "VA | Abilities | Scanner")
	float ScanRadius = 1000.f;

	// time that the objects remain marked
	UPROPERTY(EditAnywhere, Category = "VA | Abilities | Scanner")
	float ScanDuration = 5.0f;
#pragma endregion

#pragma region ASSAULT PROTOCOL
protected:
	// The projectile that C0M-P4 will shoot
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VA | Orders | Order 1")
	TSubclassOf<AVA_BaseProjectile> ProjectileClass;

	// Firing rate interval in seconds
	UPROPERTY(EditAnywhere, Category = "VA | Abilities | Assault")
	float FireRate = 0.2f;

  // How long the drone will keep shooting at the target before re-evaluating
	UPROPERTY(EditAnywhere, Category = "VA | Abilities | Assault")
  float AssaultDuration = 5.f;

private:
	// Timer for auto shoot
	FTimerHandle AssaultTH;

	// Timer to automatically shut down the assault mode
	FTimerHandle DurationTH;

	// Internal loop function executed by the timer
	void AutomatedAssaultLoop();

	// The current target validated by Line of Sight
	AActor* CurrentCombatTarget;

	// Upgraded target acquisition tracking with Line of Sight checks
	AActor* GetBestVisibleTarget();

	// Rotates the drone toward the current target smoothly
	void RotateTowardsTarget(float DeltaTime);

	// Bool to keep rotation focus on enemy in assault mode
	bool bInAssaultMode = false;
#pragma endregion

#pragma region GAS PROTOCOL
protected:
	// GasCloud class
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VA | Orders | Order 2")
	TSubclassOf<AVA_GasCloud> GasCloudClass;
#pragma endregion

#pragma region REPAIR PROTOCOL
protected:
	// Health amount per sec
	UPROPERTY(EditAnywhere, Category = "VA | Abilities | Repair")
	float HealthPerTick = 10.f;

	// Internal function to apply health
	void ApplyRepairTick();

private:
	// Timer for reparation
	FTimerHandle RepairTH;

	// How many times apply the health (ticks)
	int32 RemainingRepairTicks = 0;
#pragma endregion

#pragma region DISTRACTION PROTOCOL
protected:
	UPROPERTY(EditAnywhere, Category = "VA | Abilities | Distraction")
  float DistractionDuration = 6.0f;

	UPROPERTY(EditAnywhere, Category = "VA | Abilities | Distraction")
  float OrbitRadius = 100.f;

	UPROPERTY(EditAnywhere, Category = "VA | Abilities | Distraction")
  float OrbitSpeed = 150.f;

private:
	FTimerHandle DistractionTH;

	bool bIsDistracting = false;
	FVector DistractionTargetLocation;

	// Helper function to manage the distraction state location update in Tick
	void HandleDistractionMovement(float DeltaTime);

	void ExecuteFakeReturn(float DeltaTime);

#pragma endregion

private:
	bool bIsCompanionBusy = false;
	bool bIsAssaultOnCD = false;
	bool bIsGasOnCD = false;
	bool bIsRepairOnCD = false;
	bool bIsDistractOnCD = false;


	FTimerHandle CooldownTH;

	// Orbit angle tracker
	float CurrentOrbitAngle = 0.0f;
	AActor* CurrentDistractionTarget = nullptr;

	// --- Return Protocol ---
	bool bIsReturningHome = false;
	int32 ReturnStage = 0; // 0: Idle/Active, 1: Ascending, 2: Teleport & Descend
	FVector ReturnTargetLocation;

	void ResetAssaultCooldown() { bIsAssaultOnCD = false; }
	void ResetGasCooldown() { bIsGasOnCD = false; }
	void ResetRepairCooldown() { bIsRepairOnCD = false; }
	void ResetDistractCooldown() { bIsDistractOnCD = false; }

	void HandleBasicFollow(float DeltaTime);

#pragma endregion
};
