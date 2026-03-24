// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "VA_Companion.generated.h"

class AVA_Character;

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
#pragma endregion

#pragma region GAMEPLAY
public:
	void ExecuteScan();
	void ExecuteOrder(int32 OrderID);
#pragma endregion
};
