// Fill out your copyright notice in the Description page of Project Settings.


#include "Anims/ANS_ToggleCollision.h"
#include "Character/VA_Character.h"

void UANS_ToggleCollision::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	if (MeshComp && MeshComp->GetOwner())
	{
		if (AVA_Character* Character = Cast<AVA_Character>(MeshComp->GetOwner()))
		{
			Character->SetAttackCollisionEnabled(true);
		}
	}
}

void UANS_ToggleCollision::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	if (MeshComp && MeshComp->GetOwner())
	{
		if (AVA_Character* Character = Cast<AVA_Character>(MeshComp->GetOwner()))
		{
			Character->SetAttackCollisionEnabled(false);
		}
	}
}
