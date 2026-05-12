// EnemyAIController.cpp

#include "EnemyAIController.h"
#include "EnemyCharacter.h"
#include "Kismet/GameplayStatics.h"

// AIController 기본값 설정
AEnemyAIController::AEnemyAIController()
{
	PrimaryActorTick.bCanEverTick = true;

	TargetPlayer = nullptr;
	ControlledEnemy = nullptr;
	CurrentState = EEnemyAIState::Idle;
	LastAttackTime = 0.f;
}

// 적 캐릭터를 조종할 때 참조와 플레이어 참조 설정
void AEnemyAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	ControlledEnemy = Cast<AEnemyCharacter>(InPawn);
	FindPlayer();
}

// 매 프레임 AI 판단 실행
void AEnemyAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdateAI();
}

// 현재 월드의 플레이어 Pawn 찾기
void AEnemyAIController::FindPlayer()
{
	TargetPlayer = UGameplayStatics::GetPlayerPawn(this, 0);
}

// 거리와 상태에 따라 추적, 공격, 대기 처리
void AEnemyAIController::UpdateAI()
{
	if (!ControlledEnemy)
	{
		return;
	}

	if (!TargetPlayer)
	{
		FindPlayer();
		return;
	}

	if (ControlledEnemy->IsDead())
	{
		ChangeState(EEnemyAIState::Dead);
		StopMovement();
		return;
	}

	const float Distance = FVector::Dist(
		ControlledEnemy->GetActorLocation(),
		TargetPlayer->GetActorLocation()
	);

	if (Distance <= ControlledEnemy->AttackRange)
	{
		ChangeState(EEnemyAIState::Attack);
		StopMovement();
		PerformAttack();
	}
	else if (Distance <= ControlledEnemy->DetectionRange)
	{
		ChangeState(EEnemyAIState::Chase);
		MoveToActor(TargetPlayer);
	}
	else
	{
		ChangeState(EEnemyAIState::Idle);
		StopMovement();
	}
}

// 같은 상태면 무시하고, 다르면 새 상태로 변경
void AEnemyAIController::ChangeState(EEnemyAIState NewState)
{
	if (CurrentState == NewState)
	{
		return;
	}

	CurrentState = NewState;
}

// 공격 쿨타임이 지났는지 확인
bool AEnemyAIController::CanAttack() const
{
	if (!ControlledEnemy || !GetWorld())
	{
		return false;
	}

	const float CurrentTime = GetWorld()->GetTimeSeconds();
	return CurrentTime - LastAttackTime >= ControlledEnemy->AttackCooldown;
}

// 공격 가능하면 공격 시간 갱신 후 로그 출력
void AEnemyAIController::PerformAttack()
{
	if (!CanAttack() || !GetWorld())
	{
		return;
	}

	LastAttackTime = GetWorld()->GetTimeSeconds();

	UE_LOG(LogTemp, Warning, TEXT("Enemy Attack"));
}
