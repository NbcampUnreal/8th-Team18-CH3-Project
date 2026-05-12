// EnemyAIController.cpp

#include "EnemyAIController.h"
#include "EnemyCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"

// AIController 기본값 설정
AEnemyAIController::AEnemyAIController()
{
	PrimaryActorTick.bCanEverTick = false;

	TargetPlayer = nullptr;
	ControlledEnemy = nullptr;
}

// 적 캐릭터를 조종할 때 참조와 플레이어 참조 설정
void AEnemyAIController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);

    ControlledEnemy = Cast<AEnemyCharacter>(InPawn);
    if (!ControlledEnemy)
    {
        return;
    }

    if (!ControlledEnemy->BehaviorTree)
    {
        UE_LOG(LogTemp, Warning, TEXT("BehaviorTree is null"));
        return;
    }

    RunBehaviorTree(ControlledEnemy->BehaviorTree);

    GetWorldTimerManager().SetTimer(
        FindPlayerTimerHandle,
        this,
        &AEnemyAIController::SetTargetPlayer,
        0.2f,
        false
    );
}

// 현재 월드의 플레이어 Pawn 찾기
void AEnemyAIController::FindPlayer()
{
	TargetPlayer = UGameplayStatics::GetPlayerPawn(this, 0);
}

void AEnemyAIController::SetTargetPlayer()
{
    FindPlayer();

    UE_LOG(LogTemp, Warning, TEXT("TargetPlayer: %s"), *GetNameSafe(TargetPlayer));

    UBlackboardComponent* BB = GetBlackboardComponent();
    if (!BB)
    {
        UE_LOG(LogTemp, Warning, TEXT("BlackboardComponent is null"));
        return;
    }

    if (!TargetPlayer)
    {
        UE_LOG(LogTemp, Warning, TEXT("TargetPlayer is still None"));
        return;
    }

    BB->SetValueAsObject(TEXT("TargetActor"), TargetPlayer);

    UE_LOG(LogTemp, Warning, TEXT("Blackboard TargetActor Set: %s"),
        *GetNameSafe(TargetPlayer));
}