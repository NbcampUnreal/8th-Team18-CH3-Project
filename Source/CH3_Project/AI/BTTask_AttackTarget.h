// BTTask_AttackTarget.h

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_AttackTarget.generated.h"

UCLASS()
class CH3_PROJECT_API UBTTask_AttackTarget : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_AttackTarget();

protected:
	// Behavior Tree가 공격 Task를 실행할 때 호출
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
