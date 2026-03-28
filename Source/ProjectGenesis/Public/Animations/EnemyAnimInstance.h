// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "EnemyAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTGENESIS_API UEnemyAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float CurrentSpeed{ 0.0f };

	UFUNCTION(BlueprintCallable)
	void UpdateSpeed();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsCharging{ false };
};
