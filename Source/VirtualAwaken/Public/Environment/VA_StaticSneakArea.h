// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Environment/VA_SneakArea.h"
#include "VA_StaticSneakArea.generated.h"

/**
 * 
 */
UCLASS()
class VIRTUALAWAKEN_API AVA_StaticSneakArea : public AVA_SneakArea
{
	GENERATED_BODY()
	
public:
	AVA_StaticSneakArea();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "VA | Components")
	TObjectPtr<class UStaticMeshComponent> MeshComp;
};
