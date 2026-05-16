// EnemyCharacter.h

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnemyCharacter.generated.h"

class UBehaviorTree;

UCLASS()
class CH3_PROJECT_API AEnemyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AEnemyCharacter();

	// 최대 HP
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy|Stat")
	float MaxHP;

	// 현재 HP
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Enemy|Stat")
	float CurrentHP;

	// 방어력. 받은 데미지에서 이 값을 뺀 뒤 HP를 감소시킴
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy|Stat")
	float Defense;

	// 적 처치 시 지급할 점수
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy|Reward")
	int32 ScoreValue;

	// 적이 플레이어에게 주는 공격 데미지
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy|Attack")
	float AttackDamage;

	// 플레이어를 TargetActor로 인식할 최대 거리
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy|AI")
	float DetectionRange;

	// 공격 Task를 실행할 수 있는 거리
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy|AI")
	float AttackRange;

	// AIController가 실행할 Behavior Tree
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy|AI")
	UBehaviorTree* BehaviorTree;

	// 공격 쿨타임
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy|Attack")
	float AttackCooldown;

	// BP_EnemyCharacter에서 공격 몽타주를 재생하기 위한 이벤트
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Enemy|Animation")
	void PlayAttackAnimation();

	// BP_EnemyCharacter에서 사망 몽타주를 재생하기 위한 이벤트
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Enemy|Animation")
	void PlayDeathAnimation();

	// Unreal 기본 데미지 시스템으로 받은 데미지를 적 HP에 반영
	virtual float TakeDamage(
		float DamageAmount,
		struct FDamageEvent const& DamageEvent,
		AController* EventInstigator,
		AActor* DamageCauser) override;

	// 방어력 계산 후 실제 HP를 감소시키는 함수
	UFUNCTION(BlueprintCallable)
	void TakeDamageFromEnemy(float Damage);

	// 현재 적이 사망 상태인지 확인
	bool IsDead() const;

	// 공격 쿨타임이 끝났는지 확인
	bool CanAttack() const;

	// 공격 시작 시간을 기록하여 쿨타임 계산에 사용
	void MarkAttack();

	// 공격 몽타주의 AttackHit Notify 시점에 플레이어에게 데미지 적용
	UFUNCTION(BlueprintCallable, Category = "Enemy|Attack")
	void ApplyAttackDamage();

protected:
	// 게임 시작 시 초기화
	virtual void BeginPlay() override;

	// 사망 처리
	void Die();

private:
	// 중복 사망 처리를 막기 위한 상태값
	bool bIsDead;

	// 마지막 공격 시작 시간
	float LastAttackTime;
};
