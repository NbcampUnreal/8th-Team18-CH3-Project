#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CombatComponent.generated.h"

// 💡 UI 연동을 위한 델리게이트(신호기) 선언
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnHitTargetDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnKillTargetDelegate);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class CH3_PROJECT_API UCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UCombatComponent();
	virtual void BeginPlay() override;

	// === 전투 기능들 ===
	void FireWeapon();        // 기본 공격
	void FireGrenade();       // 유탄 발사
	void Reload();            // 장전 기능

	// === 🎯 UI 신호기 (블루프린트에서 바인딩 가능) ===
	UPROPERTY(BlueprintAssignable, Category = "Combat|UI")
	FOnHitTargetDelegate OnHitTarget;

	UPROPERTY(BlueprintAssignable, Category = "Combat|UI")
	FOnKillTargetDelegate OnKillTarget;

	// === 탄창 및 상태 변수 ===
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	int32 CurrentAmmo = 30;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	bool bIsReloading = false;

	// === 유탄 스탯 및 쿨타임 ===
	UPROPERTY(EditAnywhere, Category = "Combat|Skill - Grenade")
	float GrenadeDamage = 50.0f;

	UPROPERTY(EditAnywhere, Category = "Combat|Skill - Grenade")
	float GrenadeRadius = 300.0f;

	UPROPERTY(EditAnywhere, Category = "Combat|Skill - Grenade")
	float GrenadeCooldown = 5.0f;

private:
	// 🔫 기본 공격 스탯 & 쿨타임
	UPROPERTY(EditAnywhere, Category = "Combat|Basic")
	float BaseDamage = 20.0f;

	UPROPERTY(EditAnywhere, Category = "Combat|Basic")
	float FireCooldown = 0.5f;

	bool bCanFire;
	FTimerHandle FireTimer;
	void ResetFire();

	// 💣 유탄 쿨타임 변수
	bool bCanUseGrenade;
	FTimerHandle GrenadeTimer;
	void ResetGrenadeCooldown();
};