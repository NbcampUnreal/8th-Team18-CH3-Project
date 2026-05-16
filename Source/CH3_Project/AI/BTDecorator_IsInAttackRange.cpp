// BTDecorator_IsInAttackRange.cpp

#include "AI/BTDecorator_IsInAttackRange.h"
#include "AI/EnemyCharacter.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

// Behavior Tree에서 보일 노드 이름 설정
UBTDecorator_IsInAttackRange::UBTDecorator_IsInAttackRange()
{
	NodeName = TEXT("Is In Attack Range");
}

// TargetActor가 적의 공격 범위 안에 있는지 확인
bool UBTDecorator_IsInAttackRange::CalculateRawConditionValue(
	UBehaviorTreeComponent& OwnerComp,
	uint8* NodeMemory
) const
{
	// 현재 Behavior Tree를 실행 중인 AIController 가져오기
	const AAIController* AIController = OwnerComp.GetAIOwner();
	if (!AIController)
	{
		return false;
	}

	// AIController가 조종 중인 Pawn을 EnemyCharacter로 변환
	const AEnemyCharacter* Enemy = Cast<AEnemyCharacter>(AIController->GetPawn());
	if (!Enemy || Enemy->IsDead())
	{
		return false;
	}

	// Blackboard에서 TargetActor 가져오기
	const UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (!BlackboardComp)
	{
		return false;
	}

	const AActor* TargetActor = Cast<AActor>(
		BlackboardComp->GetValueAsObject(TEXT("TargetActor"))
	);

	// 감지된 타겟이 없으면 공격 범위 조건 실패
	if (!TargetActor)
	{
		return false;
	}

	// 적과 타겟 사이의 거리를 계산
	const float Distance = FVector::Dist(
		Enemy->GetActorLocation(),
		TargetActor->GetActorLocation()
	);

	// 공격 범위 안이면 true, 아니면 false
	return Distance <= Enemy->AttackRange;
}
