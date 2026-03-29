// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NPCs/VA_BaseNPC.h"
#include "VA_BaseEnemy.generated.h"

/**
 * 
 */
UCLASS()
class VIRTUALAWAKEN_API AVA_BaseEnemy : public AVA_BaseNPC
{
	GENERATED_BODY()
	
public:
	AVA_BaseEnemy();

protected:
	virtual void BeginPlay() override;
};
