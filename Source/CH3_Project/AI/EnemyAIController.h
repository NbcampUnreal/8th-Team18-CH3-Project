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
	// 추적할 플레이어
	UPROPERTY()
	APawn* TargetPlayer;

	// 현재 조종 중인 적 캐릭터
	UPROPERTY()
	class AEnemyCharacter* ControlledEnemy;

	FTimerHandle FindPlayerTimerHandle;

	// 플레이어 찾기
	void FindPlayer();

	void SetTargetPlayer();
};
