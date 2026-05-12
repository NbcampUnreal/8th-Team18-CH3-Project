// EnemyAIController.h

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "EnemyAIController.generated.h"

UENUM(BlueprintType)
enum class EEnemyAIState : uint8
{
	// 대기 상태
	Idle,
	// 추적 상태
	Chase,
	// 공격 상태
	Attack,
	// 사망 상태
	Dead
};

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
	// 매 프레임 AI 판단 실행
	virtual void Tick(float DeltaTime) override;

private:
	// 추적할 플레이어
	UPROPERTY()
	APawn* TargetPlayer;

	// 현재 조종 중인 적 캐릭터
	UPROPERTY()
	class AEnemyCharacter* ControlledEnemy;

	// 현재 AI 상태
	EEnemyAIState CurrentState;
	// 마지막 공격 시간
	float LastAttackTime;

	// 플레이어 찾기
	void FindPlayer();
	// AI 상태와 행동 갱신
	void UpdateAI();
	// AI 상태 변경
	void ChangeState(EEnemyAIState NewState);
	// 공격 가능한지 확인
	bool CanAttack() const;
	// 공격 실행
	void PerformAttack();
};
