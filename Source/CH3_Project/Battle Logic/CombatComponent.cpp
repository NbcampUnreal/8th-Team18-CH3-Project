#include "CombatComponent.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/Character.h"
#include "GameFramework/PlayerController.h" 
#include "TimerManager.h" 

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
	if (!bCanFire || bIsReloading || CurrentAmmo <= 0)
	{
		if (CurrentAmmo <= 0) UE_LOG(LogTemp, Warning, TEXT("탄창이 비었습니다! R키로 장전하세요."));
		return;
	}

	CurrentAmmo--;
	UE_LOG(LogTemp, Log, TEXT("남은 탄창: %d"), CurrentAmmo);

	bCanFire = false;
	GetWorld()->GetTimerManager().SetTimer(FireTimer, this, &UCombatComponent::ResetFire, FireCooldown, false);

	AActor* Owner = GetOwner();
	ACharacter* CharOwner = Cast<ACharacter>(Owner);
	if (!Owner || !CharOwner) return;

	APlayerController* PC = Cast<APlayerController>(CharOwner->GetController());
	if (!PC) return;

	// 🎯 [핵심 보정] 실제 조준 판정은 카메라(화면 정중앙) 위치에서 시작합니다.
	FVector CameraLoc;
	FRotator CameraRot;
	PC->GetPlayerViewPoint(CameraLoc, CameraRot);

	FVector TraceStart = CameraLoc;
	FVector TraceEnd = TraceStart + (CameraRot.Vector() * 5000.f);

	FHitResult HitResult;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(Owner);

	// 1. 카메라에서 정면으로 레이저 판정
	bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECC_Visibility, Params);

	// 2. 눈에 보이는 빨간 선의 시작점은 '총구'로 설정
	FVector MuzzleLocation;
	if (CharOwner->GetMesh()->DoesSocketExist(TEXT("WeaponSocket")))
		MuzzleLocation = CharOwner->GetMesh()->GetSocketLocation(TEXT("WeaponSocket"));
	else
		MuzzleLocation = Owner->GetActorLocation();

	FVector VisualEnd = bHit ? HitResult.ImpactPoint : TraceEnd;
	DrawDebugLine(GetWorld(), MuzzleLocation, VisualEnd, FColor::Red, false, 1.0f, 0, 2.0f);

	// 3. 데미지 적용 및 UI 신호 발송
	if (bHit)
	{
		AActor* HitActor = HitResult.GetActor();
		if (HitActor)
		{
			UGameplayStatics::ApplyDamage(HitActor, BaseDamage, Owner->GetInstigatorController(), Owner, UDamageType::StaticClass());

			// UI에게 적중 신호 전달
			OnHitTarget.Broadcast();
		}
	}
}
void UCombatComponent::ResetFire() { bCanFire = true; }

// 💣 유탄 발사
void UCombatComponent::FireGrenade()
{
	if (!bCanUseGrenade || bIsReloading || CurrentAmmo <= 0) return;

	CurrentAmmo--;

	bCanUseGrenade = false;
	GetWorld()->GetTimerManager().SetTimer(GrenadeTimer, this, &UCombatComponent::ResetGrenadeCooldown, GrenadeCooldown, false);

	AActor* Owner = GetOwner();
	if (!Owner) return;

	ACharacter* CharOwner = Cast<ACharacter>(Owner);
	FVector ShootDirection = Owner->GetActorForwardVector();
	FVector BaseStart = Owner->GetActorLocation();

	if (CharOwner)
	{
		if (APlayerController* PC = Cast<APlayerController>(CharOwner->GetController()))
		{
			FVector CameraLoc;
			FRotator CameraRot;
			PC->GetPlayerViewPoint(CameraLoc, CameraRot);
			ShootDirection = CameraRot.Vector();
			BaseStart = CameraLoc;
		}
	}

	FVector ExplodeLocation = BaseStart + (ShootDirection * 400.f);
	DrawDebugSphere(GetWorld(), ExplodeLocation, GrenadeRadius, 16, FColor::Purple, false, 2.0f, 0, 1.5f);

	TArray<AActor*> IgnoreActors;
	IgnoreActors.Add(Owner);

	UGameplayStatics::ApplyRadialDamage(GetWorld(), GrenadeDamage, ExplodeLocation, GrenadeRadius, UDamageType::StaticClass(), IgnoreActors, Owner, Owner->GetInstigatorController(), true);
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
			UE_LOG(LogTemp, Log, TEXT("장전 완료!"));
		}, 1.5f, false);
}