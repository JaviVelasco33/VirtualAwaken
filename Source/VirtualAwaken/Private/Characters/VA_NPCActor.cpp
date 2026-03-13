// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/VA_NPCActor.h"

// Sets default values
AVA_NPCActor::AVA_NPCActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AVA_NPCActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AVA_NPCActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AVA_NPCActor::OnInteract_Implementation(AActor* Interactor)
{

}

void AVA_NPCActor::ShowDialogue_Implementation()
{

}
