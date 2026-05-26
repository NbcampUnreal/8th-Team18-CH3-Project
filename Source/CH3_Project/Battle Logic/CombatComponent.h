#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CombatComponent.generated.h"

// 💡 UI 연동을 위한 델리게이트(신호기) 선언
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnHitTargetDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnKillTargetDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAmmoChangedDelegate, int32, NewAmmo); // 팀원 UI를 위한 총알 변경 알림 추가

class UParticleSystem;
class USoundBase;
class AGrenadeProjectile;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class CH3_PROJECT_API UCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UCombatComponent();
	virtual void BeginPlay() override;

	// === 전투 기능들 ===
	UFUNCTION(BlueprintCallable, Category = "Combat")
	void FireWeapon();        // 기본 공격

	UFUNCTION(BlueprintCallable, Category = "Combat")
	void FireGrenade();       // 유탄 발사

	UFUNCTION(BlueprintCallable, Category = "Combat")
	void Reload();            // 장전 기능

	// 🎯 적이 죽었을 때 외부(적 블루프린트 등)에서 호출해 줄 킬 확인 함수
	UFUNCTION(BlueprintCallable, Category = "Combat|UI")
	void NotifyKill();

	// === 🎯 UI 신호기 (팀원 위젯 블루프린트에서 바인딩해서 사용) ===
	UPROPERTY(BlueprintAssignable, Category = "Combat|UI")
	FOnHitTargetDelegate OnHitTarget;

	UPROPERTY(BlueprintAssignable, Category = "Combat|UI")
	FOnKillTargetDelegate OnKillTarget;

	UPROPERTY(BlueprintAssignable, Category = "Combat|UI")
	FOnAmmoChangedDelegate OnAmmoChanged;

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

	// ===========공격 이펙트, 사운드=========== //
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat|FX")
	UParticleSystem* WeaponEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat|FX")
	USoundBase* WeaponSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat|FX")
	float WeaponSoundVolume = 0.3f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat|Skill - Grenade")
	TSubclassOf<AGrenadeProjectile> GrenadeProjectileClass;
	// ===========공격 이펙트, 사운드=========== //

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