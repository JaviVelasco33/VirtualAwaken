// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Environment/VA_SneakArea.h"
#include "VA_GasCloud.generated.h"

/**
 * 
 */
UCLASS()
class VIRTUALAWAKEN_API AVA_GasCloud : public AVA_SneakArea
{
	GENERATED_BODY()
	
public:
	AVA_GasCloud();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "VA | Components")
	TObjectPtr<class UParticleSystemComponent> SmokeParticles;
};
