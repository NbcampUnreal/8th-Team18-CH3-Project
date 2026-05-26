// EnemyCharacter.cpp

#include "EnemyCharacter.h"
#include "CH3_Project/ShooterGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "MainCharacter.h"
#include "Engine/Engine.h"

// 적 기본 스탯과 이동 회전 설정
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
	LastAttackTime = -AttackCooldown;
	bIsDead = false;

	// 여러 적이 서로 겹치며 이동하지 않도록 RVO 회피 사용
	GetCharacterMovement()->bUseRVOAvoidance = true;
	GetCharacterMovement()->AvoidanceConsiderationRadius = 300.f;

	// 이동 중에는 이동 방향을 바라보도록 설정
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 720.f, 0.f);
}

// 시작 시 현재 HP를 최대 HP로 초기화
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

// 받은 데미지에서 방어력을 뺀 뒤 HP를 감소시킴
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

// 마지막 공격 이후 쿨타임이 지났는지 확인
bool AEnemyCharacter::CanAttack() const
{
	const UWorld* World = GetWorld();
	if (!World)
	{
		return false;
	}

	return World->GetTimeSeconds() - LastAttackTime >= AttackCooldown;
}

// 현재 시간을 마지막 공격 시간으로 저장
void AEnemyCharacter::MarkAttack()
{
	const UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	LastAttackTime = World->GetTimeSeconds();
}

// 공격 몽타주의 AttackHit Notify에서 호출되어 실제 데미지를 적용
void AEnemyCharacter::ApplyAttackDamage()
{
	if (bIsDead)
	{
		return;
	}

	// Blackboard에 접근하기 위해 현재 Enemy를 조종하는 AIController를 가져옴
	AAIController* AIController = Cast<AAIController>(GetController());
	if (!AIController)
	{
		return;
	}

	// TargetActor는 EnemyAIController가 감지 범위 안의 플레이어를 넣어둔 값
	UBlackboardComponent* BlackboardComp = AIController->GetBlackboardComponent();
	if (!BlackboardComp)
	{
		return;
	}

	AActor* TargetActor = Cast<AActor>(
		BlackboardComp->GetValueAsObject(TEXT("TargetActor"))
	);

	if (!TargetActor)
	{
		return;
	}

	// Notify가 늦게 호출되는 동안 플레이어가 멀리 도망간 경우 데미지를 주지 않음
	const float DistanceToTarget = FVector::Dist2D(
		GetActorLocation(),
		TargetActor->GetActorLocation()
	);

	if (DistanceToTarget > AttackRange + 50.f)
	{
		return;
	}

	// TargetActor는 플레이어이므로 플레이어의 TakeDamage가 호출됨
	UGameplayStatics::ApplyDamage(
		TargetActor,
		AttackDamage,
		AIController,
		this,
		nullptr
	);

	// 테스트용으로 적이 데미지를 준 직후 플레이어 HP를 화면에 출력
	if (AMainCharacter* MainCharacter = Cast<AMainCharacter>(TargetActor))
	{
		if (GEngine)
		{
			const FString HPText = FString::Printf(
				TEXT("%.0f/%.0f"),
				MainCharacter->CurrentPlayerHP,
				MainCharacter->PlayerMaxHP
			);

			GEngine->AddOnScreenDebugMessage(
				-1,
				2.0f,
				FColor::Red,
				HPText
			);
		}
	}
}

// 점수 지급, 이동/충돌 비활성화, 사망 몽타주 재생 후 액터 제거
void AEnemyCharacter::Die()
{
	if (bIsDead)
	{
		return;
	}

	bIsDead = true;

	// 처치 점수 지급
	AShooterGameMode* GM = Cast<AShooterGameMode>(UGameplayStatics::GetGameMode(this));
	if (GM)
	{
		GM->AddScore(ScoreValue);
	}

	// 사망 후 이동과 충돌을 막아 AI가 더 이상 움직이거나 막히지 않게 함
	GetCharacterMovement()->DisableMovement();
	GetCharacterMovement()->StopMovementImmediately();
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// BP_EnemyCharacter에서 연결한 사망 몽타주 재생
	PlayDeathAnimation();

	// 사망 애니메이션이 보일 시간을 준 뒤 액터 제거
	SetLifeSpan(2.5f);
}
