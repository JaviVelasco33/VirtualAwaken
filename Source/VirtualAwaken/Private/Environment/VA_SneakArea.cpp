// Fill out your copyright notice in the Description page of Project Settings.


#include "Environment/VA_SneakArea.h"
#include "Character/VA_Character.h"
#include "Components/ShapeComponent.h"
#include "Components/SceneComponent.h"
#include "Components/BoxComponent.h"

// Sets default values
AVA_SneakArea::AVA_SneakArea()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AreaCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("AreaCollision"));
	RootComponent = AreaCollision;

	AreaCollision->SetCollisionProfileName(TEXT("Trigger"));

	AreaCollision->OnComponentBeginOverlap.AddDynamic(this, &AVA_SneakArea::OnOverlapBegin);
	AreaCollision->OnComponentEndOverlap.AddDynamic(this, &AVA_SneakArea::OnOverlapEnd);
}

// Called when the game starts or when spawned
void AVA_SneakArea::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AVA_SneakArea::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

#pragma region OVERLAPS
void AVA_SneakArea::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Sweepresult)
{
	if (AVA_Character* Player = Cast<AVA_Character>(OtherActor))
	{
		Player->SetInsideHiddingZone(true);
	}
}

void AVA_SneakArea::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (AVA_Character* Player = Cast<AVA_Character>(OtherActor))
	{
		Player->SetInsideHiddingZone(false);
	}
}

#pragma endregion