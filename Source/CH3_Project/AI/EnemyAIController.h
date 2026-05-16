// EnemyAIController.h

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "TimerManager.h"
#include "EnemyAIController.generated.h"

UCLASS()
class CH3_PROJECT_API AEnemyAIController : public AAIController
{
	GENERATED_BODY()

public:
	// 생성자
	AEnemyAIController();

protected:
	// 적 캐릭터를 조종하기 시작할 때 호출
	virtual void OnPossess(APawn* InPawn) override;

private:
	// 현재 추적 중인 플레이어 Pawn
	UPROPERTY()
	APawn* TargetPlayer;

	// 현재 조종 중인 적 캐릭터
	UPROPERTY()
	class AEnemyCharacter* ControlledEnemy;

	// 주기적으로 플레이어를 찾아 Blackboard에 갱신하기 위한 타이머
	FTimerHandle FindPlayerTimerHandle;

	// 현재 월드의 플레이어 Pawn을 찾음
	void FindPlayer();

	// 감지 범위에 따라 Blackboard의 TargetActor를 설정하거나 제거
	void SetTargetPlayer();
};
