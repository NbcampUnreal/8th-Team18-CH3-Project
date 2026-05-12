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

	// 방어력
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy|Stat")
	float Defense;

	// 처치 시 획득 점수
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy|Reward")
	int32 ScoreValue;

	// 공격 데미지
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy|Attack")
	float AttackDamage;

	// 플레이어 감지 범위
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy|AI")
	float DetectionRange;

	// 공격 가능 범위
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy|AI")
	float AttackRange;

	// BehaviorTree 연결
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy|AI")
	UBehaviorTree* BehaviorTree;

	// 공격 쿨타임
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy|Attack")
	float AttackCooldown;

	// Unreal 기본 데미지 시스템으로 받은 데미지 처리
	virtual float TakeDamage(
		float DamageAmount,
		struct FDamageEvent const& DamageEvent,
		AController* EventInstigator,
		AActor* DamageCauser) override;

	// 데미지 처리
	UFUNCTION(BlueprintCallable)
	void TakeDamageFromEnemy(float Damage);

	// 사망 여부 확인
	bool IsDead() const;

protected:
	// 게임 시작 시 초기화
	virtual void BeginPlay() override;

	// 사망 처리
	void Die();

private:
	// 사망 상태
	bool bIsDead;
};
