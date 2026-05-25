#include "CombatComponent.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/Character.h"
#include "GameFramework/PlayerController.h" 
#include "TimerManager.h" 
#include "GrenadeProjectile.h"
#include "Particles/ParticleSystemComponent.h"

UCombatComponent::UCombatComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	bCanFire = true;
	bCanUseGrenade = true;
	CurrentAmmo = 30;
	bIsReloading = false;
}

void UCombatComponent::BeginPlay()
{
	Super::BeginPlay();
}

// 🔫 기본 공격 (레이저)
void UCombatComponent::FireWeapon()
{
	// 장전 중이거나 총알이 없으면 발사 불가
	if (!bCanFire || bIsReloading || CurrentAmmo <= 0)
	{
		if (CurrentAmmo <= 0) UE_LOG(LogTemp, Warning, TEXT("탄창이 비었습니다! R키로 장전하세요."));
		return;
	}

	// 1. 탄창 감소 및 UI 알림 방송
	CurrentAmmo--;
	OnAmmoChanged.Broadcast(CurrentAmmo); // 💡 팀원 UI에 바뀐 총알 수 전달
	UE_LOG(LogTemp, Log, TEXT("남은 탄창: %d"), CurrentAmmo);

	bCanFire = false;
	GetWorld()->GetTimerManager().SetTimer(FireTimer, this, &UCombatComponent::ResetFire, FireCooldown, false);

	AActor* Owner = GetOwner();
	ACharacter* CharOwner = Cast<ACharacter>(Owner);
	if (!Owner || !CharOwner) return;

	APlayerController* PC = Cast<APlayerController>(CharOwner->GetController());
	if (!PC) return;

	// 🎯 고저차 조준 보정: 실제 충돌 계산은 카메라 화면 중앙 기준
	FVector CameraLoc;
	FRotator CameraRot;
	PC->GetPlayerViewPoint(CameraLoc, CameraRot);

	FVector TraceStart = CameraLoc;
	FVector TraceEnd = TraceStart + (CameraRot.Vector() * 5000.f);

	FHitResult HitResult;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(Owner);

	bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECC_Visibility, Params);

	// 시각용 빨간 선은 총구(WeaponSocket) 위치에서 발사
	FVector MuzzleLocation;
	if (CharOwner->GetMesh()->DoesSocketExist(TEXT("WeaponSocket")))
		MuzzleLocation = CharOwner->GetMesh()->GetSocketLocation(TEXT("WeaponSocket"));
	else
		MuzzleLocation = Owner->GetActorLocation();

	// =========== 공격 이펙트, 사운드 추가 =========== //
	if (WeaponEffect)
	{
		UParticleSystemComponent* SpawnedWeaponEffect =
			UGameplayStatics::SpawnEmitterAtLocation(
				GetWorld(),
				WeaponEffect,
				MuzzleLocation,
				CameraRot
			);

		if (SpawnedWeaponEffect)
		{
			FTimerHandle WeaponEffectTimerHandle;

			GetWorld()->GetTimerManager().SetTimer(
				WeaponEffectTimerHandle,
				[SpawnedWeaponEffect]()
				{
					if (IsValid(SpawnedWeaponEffect))
					{
						SpawnedWeaponEffect->DestroyComponent();
					}
				},
				1.0f,
				false
			);
		}
	}

	if (WeaponSound)
	{
		UGameplayStatics::PlaySoundAtLocation(
			GetWorld(),
			WeaponSound,
			MuzzleLocation,
			WeaponSoundVolume
		);
	}
	// =========== 공격 이펙트, 사운드 추가 =========== //

	FVector VisualEnd = bHit ? HitResult.ImpactPoint : TraceEnd;
	DrawDebugLine(GetWorld(), MuzzleLocation, VisualEnd, FColor::Red, false, 1.0f, 0, 2.0f);

	if (bHit)
	{
		AActor* HitActor = HitResult.GetActor();
		if (HitActor)
		{
			// 데미지 적용
			UGameplayStatics::ApplyDamage(HitActor, BaseDamage, Owner->GetInstigatorController(), Owner, UDamageType::StaticClass());

			// 🎯 UI 연동: 적 맞췄을 때 히트마커 신호 방송
			OnHitTarget.Broadcast();
		}
	}
}

void UCombatComponent::ResetFire() { bCanFire = true; }

// 🎯 적이 죽었을 때 이 함수를 호출하면 UI로 킬 신호가 방송됩니다.
void UCombatComponent::NotifyKill()
{
	OnKillTarget.Broadcast();
}

// 💣 유탄 발사
void UCombatComponent::FireGrenade()
{
	// ===================== 기존 코드 ===================== //
	//if (!bCanUseGrenade || bIsReloading || CurrentAmmo <= 0) return;

	//// 유탄 발사 시에도 총알 감소 및 UI 알림
	//CurrentAmmo--;
	//OnAmmoChanged.Broadcast(CurrentAmmo);

	//bCanUseGrenade = false;
	//GetWorld()->GetTimerManager().SetTimer(GrenadeTimer, this, &UCombatComponent::ResetGrenadeCooldown, GrenadeCooldown, false);

	//AActor* Owner = GetOwner();
	//if (!Owner) return;

	//ACharacter* CharOwner = Cast<ACharacter>(Owner);
	//FVector ShootDirection = Owner->GetActorForwardVector();
	//FVector BaseStart = Owner->GetActorLocation();

	//if (CharOwner)
	//{
	//	if (APlayerController* PC = Cast<APlayerController>(CharOwner->GetController()))
	//	{
	//		FVector CameraLoc;
	//		FRotator CameraRot;
	//		PC->GetPlayerViewPoint(CameraLoc, CameraRot);
	//		ShootDirection = CameraRot.Vector();
	//		BaseStart = CameraLoc;
	//	}
	//}

	//FVector ExplodeLocation = BaseStart + (ShootDirection * 400.f);
	//DrawDebugSphere(GetWorld(), ExplodeLocation, GrenadeRadius, 16, FColor::Purple, false, 2.0f, 0, 1.5f);

	//TArray<AActor*> IgnoreActors;
	//IgnoreActors.Add(Owner);

	//UGameplayStatics::ApplyRadialDamage(GetWorld(), GrenadeDamage, ExplodeLocation, GrenadeRadius, UDamageType::StaticClass(), IgnoreActors, Owner, Owner->GetInstigatorController(), true);

	// ===================== 기존 코드 ===================== //

	// ===================== 투사체 및 파티클 적용 코드 ===================== //
	if (!bCanUseGrenade || bIsReloading || CurrentAmmo <= 0) return;

	AActor* Owner = GetOwner();
	if (!Owner || !GrenadeProjectileClass) return;

	CurrentAmmo--;
	OnAmmoChanged.Broadcast(CurrentAmmo);

	bCanUseGrenade = false;
	GetWorld()->GetTimerManager().SetTimer(
		GrenadeTimer,
		this,
		&UCombatComponent::ResetGrenadeCooldown,
		GrenadeCooldown,
		false
	);

	ACharacter* CharOwner = Cast<ACharacter>(Owner);

	FVector SpawnLocation = Owner->GetActorLocation();
	FVector ShootDirection = Owner->GetActorForwardVector();

	if (CharOwner)
	{
		if (CharOwner->GetMesh()->DoesSocketExist(TEXT("hand_rSocket")))
		{
			SpawnLocation = CharOwner->GetMesh()->GetSocketLocation(TEXT("hand_rSocket"));
		}

		if (APlayerController* PC = Cast<APlayerController>(CharOwner->GetController()))
		{
			FVector CameraLoc;
			FRotator CameraRot;
			PC->GetPlayerViewPoint(CameraLoc, CameraRot);

			FVector TraceStart = CameraLoc;
			FVector TraceEnd = TraceStart + CameraRot.Vector() * 3000.f;

			FHitResult HitResult;
			FCollisionQueryParams Params;
			Params.AddIgnoredActor(Owner);

			bool bHit = GetWorld()->LineTraceSingleByChannel(
				HitResult,
				TraceStart,
				TraceEnd,
				ECC_Visibility,
				Params
			);

			FVector TargetPoint = bHit ? HitResult.ImpactPoint : TraceEnd;

			ShootDirection = (TargetPoint - SpawnLocation).GetSafeNormal();
		}
	}

	SpawnLocation += ShootDirection * 40.f;
	SpawnLocation += FVector(0.f, 0.f, 10.f);

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = Owner;
	SpawnParams.Instigator = Cast<APawn>(Owner);
	SpawnParams.SpawnCollisionHandlingOverride =
		ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	AGrenadeProjectile* Grenade = GetWorld()->SpawnActor<AGrenadeProjectile>(
		GrenadeProjectileClass,
		SpawnLocation,
		ShootDirection.Rotation(),
		SpawnParams
	);

	if (Grenade)
	{
		Grenade->SetExplosionData(GrenadeDamage, GrenadeRadius);
	}
	// ===================== 투사체 및 파티클 적용 코드 ===================== //
}

void UCombatComponent::ResetGrenadeCooldown() { bCanUseGrenade = true; }

// 🔄 장전 기능
void UCombatComponent::Reload()
{
	if (bIsReloading || CurrentAmmo >= 30) return;

	bIsReloading = true;
	UE_LOG(LogTemp, Log, TEXT("장전 중..."));

	FTimerHandle ReloadTimer;
	GetWorld()->GetTimerManager().SetTimer(ReloadTimer, [this]()
		{
			CurrentAmmo = 30;
			bIsReloading = false;

			// 🎯 UI 연동: 장전 완료 시 30발 알림 방송
			OnAmmoChanged.Broadcast(CurrentAmmo);

			UE_LOG(LogTemp, Log, TEXT("장전 완료!"));
		}, 1.5f, false);
}