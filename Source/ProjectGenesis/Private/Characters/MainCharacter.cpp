// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/MainCharacter.h"
#include "Characters/StatsComponent.h"
#include "Characters/EStat.h"
#include "Combat/LockonComponent.h"
#include "Combat/CombatComponent.h"
#include "Combat/TraceComponent.h"
#include "Combat/BlockComponent.h"
#include "Characters/PlayerActionsComponent.h"
#include "Animations/PlayerAnimInstance.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values
AMainCharacter::AMainCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StatsComp = CreateDefaultSubobject<UStatsComponent>(TEXT("StatsComponent"));
	LockonComp = CreateDefaultSubobject<ULockonComponent>(TEXT("LockOn"));
	CombatComp = CreateDefaultSubobject<UCombatComponent>(TEXT("CombatComponent"));
	TraceComp = CreateDefaultSubobject<UTraceComponent>(TEXT("TraceComponent"));
	BlockComp = CreateDefaultSubobject<UBlockComponent>(TEXT("BlockComponent"));
	PlayerActionsComp = CreateDefaultSubobject<UPlayerActionsComponent>(TEXT("PlayerActionComponent"));
}

// Called when the game starts or when spawned
void AMainCharacter::BeginPlay()
{
	Super::BeginPlay();

	PlayerAnim = Cast<UPlayerAnimInstance>(GetMesh()->GetAnimInstance());
	
}

// Called every frame
void AMainCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMainCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

float AMainCharacter::GetDamage()
{
	return StatsComp->Stats[EStat::Strength];
}

bool AMainCharacter::HasEnoughStamina(float Cost)
{
	return StatsComp->Stats[EStat::Stamina] >= Cost;
}

void AMainCharacter::HandleDeath()
{
	PlayAnimMontage(DeathAnimMontage);
	DisableInput(GetController<APlayerController>());
}

void AMainCharacter::EndLockonWithActor(AActor* ActorRef)
{
	if (LockonComp->CurrentTargetActor != ActorRef) { return; }
	LockonComp->EndLockon();
}

bool AMainCharacter::CanTakeDamage(AActor* Opponent)
{
	if (PlayerActionsComp->bIsRollActive) { return false; }
	if (PlayerAnim->bIsBlocking)
	{
		return BlockComp->Check(Opponent);
	}
	return true;
}

void AMainCharacter::PlayHurtAnim(TSubclassOf<class UCameraShakeBase> CameraShakeTemplate)
{
	float Duration = PlayAnimMontage(HurtAnimMontage);
	GetWorldTimerManager().SetTimer(
		HurtTimerHandle,
		this,
		&AMainCharacter::ResetActionBools,
		Duration,
		false
	);

	if (CameraShakeTemplate)
	{
		GetController<APlayerController>()
			->ClientStartCameraShake(CameraShakeTemplate);
	}
}

void AMainCharacter::ResetActionBools()
{
	CombatComp->HandleResetAttack();
	TraceComp->HandleResetAttack();
}





