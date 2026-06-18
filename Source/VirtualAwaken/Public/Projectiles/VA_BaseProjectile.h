// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

#include "VA_BaseProjectile.generated.h"

UENUM(BlueprintType)
enum class EVA_Team : uint8
{
	Ally UMETA(DisplayName = "Ally"),
	Enemy UMETA(DisplayName = "Enemy"),
	Neutral UMETA(DisplayName = "Neutral")
};

UCLASS()
class VIRTUALAWAKEN_API AVA_BaseProjectile : public AActor
{
	GENERATED_BODY()
	
#pragma region INIT
public:	
	// Sets default values for this actor's properties
	AVA_BaseProjectile();

	// Called every frame
	virtual void Tick(float DeltaTime) override;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

#pragma endregion

#pragma region COMPONENTS
public:
	UPROPERTY(VisibleAnywhere, Category = "VA | Components")
	TObjectPtr<class UBoxComponent> CollisionComp;

	UPROPERTY(VisibleAnywhere, Category = "VA | Components")
	TObjectPtr<class UStaticMeshComponent> Mesh;

  UPROPERTY(VisibleAnywhere, Category = "VA | Components")
	TObjectPtr<class UProjectileMovementComponent> ProjectileMovement;

#pragma endregion

#pragma region PROPERTIES
public:
  // Who fired this projectile?
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VA | Projectile | Team")
  EVA_Team SourceTeam = EVA_Team::Neutral;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VA | Projectile | Stats")
  float DamageValue = 20.f;

protected:
	UFUNCTION()
  virtual void OnProjectileHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
#pragma endregion
};
