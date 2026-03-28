// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/AI/BTT_Simple_MeleeAttack.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Navigation/PathFollowingComponent.h"
#include "AIController.h"
#include "Interfaces/Fighter.h"
#include "GameFramework/Character.h"
#include "Characters/EEnemyState.h"

EBTNodeResult::Type UBTT_Simple_MeleeAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	bIsFinished = false;

	float Distance{
		OwnerComp.GetBlackboardComponent()
		->GetValueAsFloat(TEXT("Distance"))
	};

	AAIController* AIRef{ OwnerComp.GetAIOwner() };

	if (Distance > AttackRadius)
	{
		APawn* PlayerRef{ GetWorld()->GetFirstPlayerController()->GetPawn() };
		FAIMoveRequest MoveRequest{ PlayerRef };

		MoveRequest.SetUsePathfinding(true);
		MoveRequest.SetAcceptanceRadius(AcceptableRadius);

		AIRef->ReceiveMoveCompleted.AddUnique(
			MoveDelegate
		);

		AIRef->MoveTo(MoveRequest);
		AIRef->SetFocus(PlayerRef);
		;
	}
	else
	{
		IFighter* FighterRef{
			Cast<IFighter>(
				AIRef->GetCharacter()
			)
		};

		FighterRef->Attack();

		FTimerHandle AttackTimerHandle;

		AIRef->GetCharacter()->GetWorldTimerManager().SetTimer(
			AttackTimerHandle,
			this,
			&UBTT_Simple_MeleeAttack::FinishAttackTask,
			FighterRef->GetAnimDuration(),
			false
		);
	}


	return EBTNodeResult::InProgress;
}

void UBTT_Simple_MeleeAttack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	float Distance{
		OwnerComp.GetBlackboardComponent()->GetValueAsFloat(TEXT("Distance"))
	};

	if (!bIsFinished)
	{
		return;
	}

	OwnerComp.GetAIOwner()->ReceiveMoveCompleted.Remove(MoveDelegate);

	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
}

UBTT_Simple_MeleeAttack::UBTT_Simple_MeleeAttack()
{
	bCreateNodeInstance = true;
	MoveDelegate.BindUFunction(this, "FinishAttackTask");

	bNotifyTick = true;
}
void UBTT_Simple_MeleeAttack::FinishAttackTask()
{
	bIsFinished = true;
}
