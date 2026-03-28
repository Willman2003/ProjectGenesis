// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/Enemy.h"
#include "Characters/EEnemyState.h"
#include "Interfaces/Fighter.h"
#include "BossCharacter.generated.h"

UCLASS()
class PROJECTGENESIS_API ABossCharacter : public ACharacter, public IEnemy, public IFighter
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere)
	TEnumAsByte<EEnemyState> InitialState;

	class UBlackboardComponent* BlackboardComp;

	UPROPERTY(EditAnywhere)
	UAnimMontage* DeathMontage;

	class AAIController* ControllerRef;

	UPROPERTY(EditAnywhere)
	UAnimMontage* HurtAnimMontage;

	FTimerHandle HurtTimerHandle;
public:
	// Sets default values for this character's properties
	ABossCharacter();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UStatsComponent* StatsComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UCombatComponent* CombatComp;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	UFUNCTION(BlueprintCallable)
	void DetectPawn(APawn* DetectedPawn, APawn* PawnToDetect, EEnemyState State);

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	virtual float GetDamage() override;

	virtual void Attack() override;

	virtual float GetAnimDuration() override;
	
	virtual float GetMeleeRange() override;

	UFUNCTION()
	void HandlePlayerDeath();
	UFUNCTION(BlueprintCallable)
	void HandleDeath();

	UFUNCTION(BlueprintCallable)
	void PlayHurtAnim();

	UFUNCTION()
	void FinishDeathAnim();
	UFUNCTION()
	void ResetActionBools();

};
