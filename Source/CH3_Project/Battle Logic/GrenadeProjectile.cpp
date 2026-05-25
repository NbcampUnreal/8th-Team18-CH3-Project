// GrenadeProjectile.cpp

#include "GrenadeProjectile.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"

AGrenadeProjectile::AGrenadeProjectile()
{
    PrimaryActorTick.bCanEverTick = false;

    CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComp"));
    CollisionComp->InitSphereRadius(12.f);
    CollisionComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    CollisionComp->SetCollisionObjectType(ECC_WorldDynamic);
    CollisionComp->SetCollisionResponseToAllChannels(ECR_Block);
    RootComponent = CollisionComp;

    MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
    MeshComp->SetupAttachment(RootComponent);
    MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
    ProjectileMovement->UpdatedComponent = CollisionComp;
    ProjectileMovement->InitialSpeed = 1000.f;
    ProjectileMovement->MaxSpeed = 1000.f;
    ProjectileMovement->ProjectileGravityScale = 1.2f;
    ProjectileMovement->bShouldBounce = true;
    ProjectileMovement->Bounciness = 0.25f;
    ProjectileMovement->Friction = 0.4f;

    InitialLifeSpan = 3.f;
}

void AGrenadeProjectile::BeginPlay()
{
    Super::BeginPlay();

    if (AActor* OwnerActor = GetOwner())
    {
        CollisionComp->IgnoreActorWhenMoving(OwnerActor, true);
    }

    GetWorldTimerManager().SetTimer(
        FuseTimerHandle,
        this,
        &AGrenadeProjectile::Explode,
        FuseTime,
        false
    );
}

void AGrenadeProjectile::SetExplosionData(float InDamage, float InRadius)
{
    ExplosionDamage = InDamage;
    ExplosionRadius = InRadius;
}

void AGrenadeProjectile::Explode()
{
    if (bHasExploded) return;
    bHasExploded = true;

    FVector ExplodeLocation = GetActorLocation();

    if (GrenadeEffect)
    {
        UGameplayStatics::SpawnEmitterAtLocation(
            GetWorld(),
            GrenadeEffect,
            ExplodeLocation
        );
    }

    if (GrenadeSound)
    {
        UGameplayStatics::PlaySoundAtLocation(
            GetWorld(),
            GrenadeSound,
            ExplodeLocation,
            GrenadeSoundVolume
        );
    }

    TArray<AActor*> IgnoreActors;
    if (GetOwner())
    {
        IgnoreActors.Add(GetOwner());
    }

    UGameplayStatics::ApplyRadialDamage(
        GetWorld(),
        ExplosionDamage,
        ExplodeLocation,
        ExplosionRadius,
        UDamageType::StaticClass(),
        IgnoreActors,
        this,
        GetInstigatorController(),
        true
    );

    Destroy();
}