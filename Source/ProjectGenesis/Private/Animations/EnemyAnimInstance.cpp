// Fill out your copyright notice in the Description page of Project Settings.


#include "Animations/EnemyAnimInstance.h"

void UEnemyAnimInstance::UpdateSpeed()
{
	APawn* PawnRef{ TryGetPawnOwner() };

	if (!IsValid(PawnRef)) {
		return;
	}

	FVector Velocity{ PawnRef->GetVelocity() };

	CurrentSpeed = static_cast<float>(Velocity.Length());
}