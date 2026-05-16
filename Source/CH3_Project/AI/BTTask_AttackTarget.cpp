// BTTask_AttackTarget.cpp

#include "AI/BTTask_AttackTarget.h"
#include "AI/EnemyCharacter.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"

// Behavior Tree에서 보일 노드 이름 설정
UBTTask_AttackTarget::UBTTask_AttackTarget()
{
	NodeName = TEXT("Attack Target");
}

// TargetActor를 바라보고 공격 몽타주를 재생하는 Task
// 실제 데미지는 몽타주의 AttackHit Notify에서 AEnemyCharacter::ApplyAttackDamage로 처리
EBTNodeResult::Type UBTTask_AttackTarget::ExecuteTask(
	UBehaviorTreeComponent& OwnerComp,
	uint8* NodeMemory
)
{
	// 현재 Behavior Tree를 실행 중인 AIController 가져오기
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!AIController)
	{
		return EBTNodeResult::Failed;
	}

	// AIController가 조종 중인 Pawn을 EnemyCharacter로 변환
	AEnemyCharacter* Enemy = Cast<AEnemyCharacter>(AIController->GetPawn());
	if (!Enemy || Enemy->IsDead())
	{
		return EBTNodeResult::Failed;
	}

	// Blackboard에서 TargetActor 가져오기
	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (!BlackboardComp)
	{
		return EBTNodeResult::Failed;
	}

	AActor* TargetActor = Cast<AActor>(
		BlackboardComp->GetValueAsObject(TEXT("TargetActor"))
	);

	if (!TargetActor)
	{
		return EBTNodeResult::Failed;
	}

	// 쿨타임 중이면 공격하지 않음
	if (!Enemy->CanAttack())
	{
		return EBTNodeResult::Failed;
	}

	// 공격 시작 전에 이동과 AI Focus를 정리
	AIController->StopMovement();
	AIController->ClearFocus(EAIFocusPriority::Gameplay);

	// 이전 이동 입력이 남아 공격 방향을 방해하지 않도록 즉시 정지
	Enemy->GetCharacterMovement()->StopMovementImmediately();

	// 플레이어 방향으로 몸 돌리기
	FVector Direction = TargetActor->GetActorLocation() - Enemy->GetActorLocation();
	Direction.Z = 0.f;

	if (!Direction.IsNearlyZero())
	{
		// Z축을 제외한 방향으로 회전하여 적이 플레이어를 바라보게 함
		FRotator LookRotation = Direction.Rotation();

		Enemy->SetActorRotation(LookRotation);
		AIController->SetControlRotation(LookRotation);
	}

	// 공격 애니메이션 재생
	Enemy->PlayAttackAnimation();

	// 공격을 시작한 시점에 쿨타임 기록
	Enemy->MarkAttack();

	// Task 성공 처리
	return EBTNodeResult::Succeeded;
}
