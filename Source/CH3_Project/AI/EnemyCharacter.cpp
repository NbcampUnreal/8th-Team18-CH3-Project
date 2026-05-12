// EnemyCharacter.cpp

#include "EnemyCharacter.h"
#include "CH3_Project/ShooterGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

// 적 기본 스탯 설정
AEnemyCharacter::AEnemyCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

	MaxHP = 100.f;
	CurrentHP = MaxHP;
	Defense = 0.f;
	ScoreValue = 1;
	AttackDamage = 10.f;
	DetectionRange = 1200.f;
	AttackRange = 150.f;
	AttackCooldown = 1.5f;
	bIsDead = false;
}

// 시작 시 현재 HP를 최대 HP로 설정
void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	CurrentHP = MaxHP;
}

// ApplyDamage로 들어온 데미지를 적 체력 시스템에 반영
float AEnemyCharacter::TakeDamage(
	float DamageAmount,
	FDamageEvent const& DamageEvent,
	AController* EventInstigator,
	AActor* DamageCauser)
{
	const float ActualDamage = Super::TakeDamage(
		DamageAmount,
		DamageEvent,
		EventInstigator,
		DamageCauser
	);

	TakeDamageFromEnemy(DamageAmount);

	return ActualDamage;
}

// 받은 데미지에서 방어력을 뺀 후 HP 감소
void AEnemyCharacter::TakeDamageFromEnemy(float Damage)
{
	if (bIsDead)
	{
		return;
	}

	const float FinalDamage = FMath::Max(Damage - Defense, 1.f);
	CurrentHP -= FinalDamage;

	if (CurrentHP <= 0.f)
	{
		Die();
	}
}

// 현재 사망 상태 반환
bool AEnemyCharacter::IsDead() const
{
	return bIsDead;
}

// 점수 지급 후 액터 제거
void AEnemyCharacter::Die()
{
	if (bIsDead)
	{
		return;
	}

	bIsDead = true;

	AShooterGameMode* GM = Cast<AShooterGameMode>(UGameplayStatics::GetGameMode(this));
	if (GM)
	{
		GM->AddScore(ScoreValue);
	}

	GetCharacterMovement()->DisableMovement();
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SetLifeSpan(2.0f);
}
