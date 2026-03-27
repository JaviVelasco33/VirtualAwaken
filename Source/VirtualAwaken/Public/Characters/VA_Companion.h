// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "VA_Companion.generated.h"

class AVA_Character;
class AVA_GasCloud;

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
	FVector OffsetFromOwner = FVector(-50.f, 50.f, 80.f);

	// Follow speed
	UPROPERTY(EditAnywhere, Category = "VA | Companion")
	float	FollowSpeed = 5.f;

	// Attribute componente (Player)
	UPROPERTY(BlueprintReadWrite, Category = "VA | Companion")
	TObjectPtr<class UVA_AttributeComponent> AttrComp;
#pragma endregion

#pragma region ABILITIES
public:
	// GasCloud class
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VA | Orders | Order 2")
	TSubclassOf<AVA_GasCloud> GasCloudClass;

	// Activate Scan
	void ExecuteScan();

	// Start hiding - Order 2
	void StartGasProtocol();

	// Start healing - Order 3
	void StartRepairProtocol();

	// To cancel the repair if player take damage
	void CancelRepair();

protected:

	// Timer for reparation
	FTimerHandle RepairTH;

	// How many times apply the health (ticks)
	int32 RemainingRepairTicks = 0;

	// Health amount per sec
	UPROPERTY(EditAnywhere, Category = "VA | Abilities")
	float HealthPerTick = 10.f;

	// Internal function to apply health
	void ApplyRepairTick();

#pragma endregion
};
