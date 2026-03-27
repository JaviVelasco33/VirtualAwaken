// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "VA_SneakArea.generated.h"

UCLASS(Abstract)
class VIRTUALAWAKEN_API AVA_SneakArea : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AVA_SneakArea();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Collision volume
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "VA | Components")
	TObjectPtr<class UShapeComponent> AreaCollision;

#pragma region OVERLAPS
protected:
	UFUNCTION()
	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Sweepresult);

	UFUNCTION()
  virtual void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
#pragma endregion

};
