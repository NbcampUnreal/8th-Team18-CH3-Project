#include "CombatComponent.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/Character.h"

UCombatComponent::UCombatComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	bCanFire = true;
	bCanUseGrenade = true;
}

void UCombatComponent::BeginPlay()
{
	Super::BeginPlay();
}

// ?? 기본 공격 발동
void UCombatComponent::FireWeapon()
{
	if (!bCanFire) return;

	bCanFire = false;
	GetWorld()->GetTimerManager().SetTimer(FireTimer, this, &UCombatComponent::ResetFire, FireCooldown, false);

	AActor* Owner = GetOwner();
	if (!Owner) return;

	FVector Start;
	ACharacter* CharOwner = Cast<ACharacter>(Owner);
	// 팀원이 만든 캐릭터에 WeaponSocket이 없다면 몸통 정중앙에서 발사됨
	if (CharOwner && CharOwner->GetMesh()->DoesSocketExist(TEXT("WeaponSocket")))
		Start = CharOwner->GetMesh()->GetSocketLocation(TEXT("WeaponSocket"));
	else
		Start = Owner->GetActorLocation();

	FVector End = Start + (Owner->GetActorForwardVector() * 2000.f);
	FHitResult HitResult;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(Owner);

	bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, Params);

	if (bHit)
	{
		DrawDebugLine(GetWorld(), Start, HitResult.ImpactPoint, FColor::Red, false, 1.0f, 0, 2.0f);
		AActor* HitActor = HitResult.GetActor();
		if (HitActor)
		{
			UGameplayStatics::ApplyDamage(HitActor, BaseDamage, Owner->GetInstigatorController(), Owner, UDamageType::StaticClass());
		}
	}
}
void UCombatComponent::ResetFire() { bCanFire = true; }

// ?? 특수 스킬(범위 폭발) 발동
void UCombatComponent::FireGrenade()
{
	if (!bCanUseGrenade) return;

	bCanUseGrenade = false;
	GetWorld()->GetTimerManager().SetTimer(GrenadeTimer, this, &UCombatComponent::ResetGrenadeCooldown, GrenadeCooldown, false);

	AActor* Owner = GetOwner();
	if (!Owner) return;

	FVector ExplodeLocation = Owner->GetActorLocation() + (Owner->GetActorForwardVector() * 400.f);

	DrawDebugSphere(GetWorld(), ExplodeLocation, GrenadeRadius, 16, FColor::Purple, false, 2.0f, 0, 1.5f);

	TArray<AActor*> IgnoreActors;
	IgnoreActors.Add(Owner);

	UGameplayStatics::ApplyRadialDamage(
		GetWorld(),
		GrenadeDamage,
		ExplodeLocation,
		GrenadeRadius,
		UDamageType::StaticClass(),
		IgnoreActors,
		Owner,
		Owner->GetInstigatorController(),
		true
	);
}
void UCombatComponent::ResetGrenadeCooldown() { bCanUseGrenade = true; }