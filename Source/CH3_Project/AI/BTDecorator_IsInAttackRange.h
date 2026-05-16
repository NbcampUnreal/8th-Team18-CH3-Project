// BTDecorator_IsInAttackRange.h

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_IsInAttackRange.generated.h"

UCLASS()
class CH3_PROJECT_API UBTDecorator_IsInAttackRange : public UBTDecorator
{
	GENERATED_BODY()

public:
	UBTDecorator_IsInAttackRange();

protected:
	// Behavior Tree가 이 Decorator 조건을 검사할 때 호출
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
};
