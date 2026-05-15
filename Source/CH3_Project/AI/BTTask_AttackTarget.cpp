// BTTask_AttackTarget.cpp

#include "AI/BTTask_AttackTarget.h"
#include "AI/EnemyCharacter.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"

 // Behavior Tree에서 보일 노드 이름 설정
UBTTask_AttackTarget::UBTTask_AttackTarget()
{
    NodeName = TEXT("Attack Target");
}

// TargetActor에게 데미지를 주는 공격 Task
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

    // 공격 중에는 이동을 멈추고 타겟을 바라보게 함
    AIController->StopMovement();
    AIController->SetFocus(TargetActor);

    // Unreal 기본 데미지 시스템으로 플레이어에게 데미지 적용
    UGameplayStatics::ApplyDamage(
        TargetActor,
        Enemy->AttackDamage,
        AIController,
        Enemy,
        nullptr
    );

    UE_LOG(
        LogTemp,
        Warning,
        TEXT("Enemy BT Attack: %.1f Damage"),
        Enemy->AttackDamage
    );

    // Task 성공 처리
    return EBTNodeResult::Succeeded;
}