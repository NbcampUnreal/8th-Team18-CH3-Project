#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CombatComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class CH3_PROJECT_API UCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UCombatComponent();
	virtual void BeginPlay() override;

	// === 외부(캐릭터)에서 호출할 전투 버튼 ===
	void FireWeapon();         // 기본 공격 (레이저 히트스캔)
	void FireGrenade();        // 특수 스킬 (유탄 범위 폭발)

private:
	// 🔫 기본 공격 스탯 & 쿨타임
	UPROPERTY(EditAnywhere, Category = "Combat|Basic")
	float BaseDamage = 20.0f;

	UPROPERTY(EditAnywhere, Category = "Combat|Basic")
	float FireCooldown = 0.5f;

	bool bCanFire;
	FTimerHandle FireTimer;
	void ResetFire();

	// 💣 유탄 스킬 스탯 & 쿨타임
	UPROPERTY(EditAnywhere, Category = "Combat|Skill - Grenade")
	float GrenadeDamage = 50.0f;

	UPROPERTY(EditAnywhere, Category = "Combat|Skill - Grenade")
	float GrenadeRadius = 300.0f;

	UPROPERTY(EditAnywhere, Category = "Combat|Skill - Grenade")
	float GrenadeCooldown = 5.0f;

	bool bCanUseGrenade;
	FTimerHandle GrenadeTimer;
	void ResetGrenadeCooldown();
};