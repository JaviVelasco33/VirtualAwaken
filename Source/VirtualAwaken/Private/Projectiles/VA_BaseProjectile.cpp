// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectiles/VA_BaseProjectile.h"
#include "NPCs/VA_BaseNPC.h"
#include "Components/SceneComponent.h"
#include "Character/VA_Character.h"

// Sets default values
AVA_BaseProjectile::AVA_BaseProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	if (!CollisionComp)
	{
		CollisionComp = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionComp"));
    RootComponent = CollisionComp;
		CollisionComp->SetNotifyRigidBodyCollision(true);
		CollisionComp->SetGenerateOverlapEvents(false);
		CollisionComp->CanCharacterStepUpOn = ECanBeCharacterBase::ECB_No;
		CollisionComp->SetCollisionProfileName(TEXT("BlockAll"));
		CollisionComp->bReturnMaterialOnMove = true;
		CollisionComp->InitBoxExtent(FVector(15.0f, 5.0f, 5.0f));

		CollisionComp->OnComponentHit.AddDynamic(this, &AVA_BaseProjectile::OnProjectileHit);
	}

	if (!Mesh)
	{
		Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
		Mesh->SetupAttachment(CollisionComp);
		Mesh->SetCollisionProfileName(TEXT("NoCollision"));
		Mesh->SetGenerateOverlapEvents(false);
		Mesh->CanCharacterStepUpOn = ECanBeCharacterBase::ECB_No;
	}

	if (!ProjectileMovement)
	{
		ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
		ProjectileMovement->UpdatedComponent = CollisionComp;

		ProjectileMovement->InitialSpeed = 1000.f;
		ProjectileMovement->MaxSpeed = 1500.f;
		ProjectileMovement->ProjectileGravityScale = 0.f;
	}

}

// Called when the game starts or when spawned
void AVA_BaseProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AVA_BaseProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AVA_BaseProjectile::OnProjectileHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (!OtherActor || OtherActor == GetOwner()) return;

  bool bShouldApplyDamage = false;

	// OtherActor is a NPC
	AVA_BaseNPC* HitNPC = Cast<AVA_BaseNPC>(OtherActor);
	if (HitNPC)
	{
    // if the projectile hits an NPC of the other team, apply damage
		if (SourceTeam == EVA_Team::Ally && HitNPC->GetNPCType() == EVA_NPCType::Enemy) bShouldApplyDamage = true;
    if (SourceTeam == EVA_Team::Enemy && HitNPC->GetNPCType() != EVA_NPCType::Enemy) bShouldApplyDamage = true;
		if (SourceTeam == EVA_Team::Neutral) bShouldApplyDamage = true;
	}
	// OtherActor is the Player
	else if (AVA_Character* Player = Cast<AVA_Character>(OtherActor))
	{
		// Enemy projectile's should apply damage to the player
		if (SourceTeam == EVA_Team::Enemy || SourceTeam == EVA_Team::Neutral) bShouldApplyDamage = true;
	}
	if (bShouldApplyDamage)
	{
		UVA_AttributeComponent* Attr = OtherActor->FindComponentByClass<UVA_AttributeComponent>();
		if (Attr)
		{
			Attr->ApplyHealthChange(-DamageValue, OtherActor);
			GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Yellow, FString::Printf(TEXT("%s damage taken"), *OtherActor->GetName()));
		}

	}
    Destroy();
}

