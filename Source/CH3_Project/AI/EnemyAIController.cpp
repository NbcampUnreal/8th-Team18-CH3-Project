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

// 적 캐릭터를 조종할 때 참조를 저장하고 Behavior Tree를 실행
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

	// EnemyCharacter에 지정된 BehaviorTree 실행
	RunBehaviorTree(ControlledEnemy->BehaviorTree);

	// 플레이어 감지 상태를 주기적으로 갱신
	GetWorldTimerManager().SetTimer(
		FindPlayerTimerHandle,
		this,
		&AEnemyAIController::SetTargetPlayer,
		0.2f,
		true
	);
}

// 현재 월드의 플레이어 Pawn 찾기
void AEnemyAIController::FindPlayer()
{
	TargetPlayer = UGameplayStatics::GetPlayerPawn(this, 0);
}

// 플레이어가 감지 범위 안에 있으면 Blackboard의 TargetActor에 저장
void AEnemyAIController::SetTargetPlayer()
{
	// 매번 현재 플레이어 Pawn을 다시 찾아 유효한 참조를 유지
	FindPlayer();

	// BehaviorTree에서 사용할 Blackboard 가져오기
	UBlackboardComponent* BB = GetBlackboardComponent();
	if (!BB)
	{
		UE_LOG(LogTemp, Warning, TEXT("BlackboardComponent is null"));
		return;
	}

	// 조종 중인 적이 없거나 이미 죽었으면 타겟 제거
	if (!ControlledEnemy || ControlledEnemy->IsDead())
	{
		BB->ClearValue(TEXT("TargetActor"));
		return;
	}

	// 플레이어를 찾지 못했으면 타겟 제거
	if (!TargetPlayer)
	{
		UE_LOG(LogTemp, Warning, TEXT("TargetPlayer is still None"));
		BB->ClearValue(TEXT("TargetActor"));
		return;
	}

	const float DistanceToPlayer = FVector::Dist2D(
		ControlledEnemy->GetActorLocation(),
		TargetPlayer->GetActorLocation()
	);

	// 감지 범위 안이면 플레이어를 TargetActor로 등록하고, 벗어나면 제거
	if (DistanceToPlayer <= ControlledEnemy->DetectionRange)
	{
		BB->SetValueAsObject(TEXT("TargetActor"), TargetPlayer);
	}
	else
	{
		BB->ClearValue(TEXT("TargetActor"));
	}
}
