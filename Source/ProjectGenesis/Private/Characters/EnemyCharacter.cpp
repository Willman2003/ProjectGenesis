// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/EnemyCharacter.h"
#include "Characters/StatsComponent.h"
#include "Combat/CombatComponent.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Characters/MainCharacter.h"
#include "BrainComponent.h"
#include "Components/CapsuleComponent.h"
#include "Interfaces/MainPlayer.h"

// Sets default values
AEnemyCharacter::AEnemyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StatsComp = CreateDefaultSubobject<UStatsComponent>(TEXT("StatsComponent"));
	CombatComp = CreateDefaultSubobject<UCombatComponent>(TEXT("CombatComponent"));
}

// Called when the game starts or when spawned
void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	ControllerRef = GetController<AAIController>();

	BlackboardComp = ControllerRef
		->GetBlackboardComponent();

	BlackboardComp->SetValueAsEnum(
		TEXT("CurrentState"),
		InitialState
	);

	GetWorld()->GetFirstPlayerController()
		->GetPawn<AMainCharacter>()
		->StatsComp
		->OnZeroHealthDelegate
		.AddDynamic(this, &AEnemyCharacter::HandlePlayerDeath);
}

void AEnemyCharacter::DetectPawn(APawn* DetectedPawn, APawn* PawnToDetect, EEnemyState State)
{
	EEnemyState CurrentState{
		static_cast<EEnemyState>(BlackboardComp->GetValueAsEnum(TEXT("CurrentState")))
	};

	if (DetectedPawn != PawnToDetect || CurrentState != EEnemyState::Idle) { return; }


	BlackboardComp->SetValueAsEnum(
		TEXT("CurrentState"),
		State
	);
}

// Called every frame
void AEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AEnemyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

float AEnemyCharacter::GetDamage()
{
	return StatsComp->Stats[EStat::Strength];
}

void AEnemyCharacter::Attack()
{
	CombatComp->RandomAttack();
}

float AEnemyCharacter::GetAnimDuration()
{
	return CombatComp->AnimDuration;
}

float AEnemyCharacter::GetMeleeRange()
{
	return StatsComp->Stats[EStat::MeleeRange];
}

void AEnemyCharacter::HandlePlayerDeath()
{
	ControllerRef->GetBlackboardComponent()
		->SetValueAsEnum(
			TEXT("CurrentState"), EEnemyState::GameOver
		);
}

void AEnemyCharacter::HandleDeath()
{
	float Duration = PlayAnimMontage(DeathMontage);

	ControllerRef->GetBrainComponent()
		->StopLogic("defeated");

	FindComponentByClass<UCapsuleComponent>()
		->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	FTimerHandle DestroyTimerHandle;

	GetWorld()->GetTimerManager().SetTimer(
		DestroyTimerHandle,
		this,
		&AEnemyCharacter::FinishDeathAnim,
		Duration,
		false
	);

	IMainPlayer* PlayerRef{
		GetWorld()->GetFirstPlayerController()
		->GetPawn<IMainPlayer>()
	};

	if (!PlayerRef) { return; }

	PlayerRef->EndLockonWithActor(this);
}

void AEnemyCharacter::FinishDeathAnim()
{
	Destroy();
}

void AEnemyCharacter::PlayHurtAnim()
{
	if (StatsComp->Stats[EStat::Health] > 0)
	{
		float Duration = PlayAnimMontage(HurtAnimMontage);
		GetWorldTimerManager().SetTimer(
			HurtTimerHandle,
			this,
			&AEnemyCharacter::ResetActionBools,
			Duration,
			false
		);
	}
}
void AEnemyCharacter::ResetActionBools()
{
	CombatComp->HandleResetAttack();
}
