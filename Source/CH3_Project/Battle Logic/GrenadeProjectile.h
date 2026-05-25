// GrenadeProjectile.h

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GrenadeProjectile.generated.h"

class USphereComponent;
class UStaticMeshComponent;
class UProjectileMovementComponent;
class UParticleSystem;
class USoundBase;

UCLASS()
class CH3_PROJECT_API AGrenadeProjectile : public AActor
{
	GENERATED_BODY()
	
public:
    AGrenadeProjectile();

    void SetExplosionData(float InDamage, float InRadius);

protected:
    virtual void BeginPlay() override;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    USphereComponent* CollisionComp;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UStaticMeshComponent* MeshComp;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UProjectileMovementComponent* ProjectileMovement;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grenade")
    float ExplosionDamage = 50.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grenade")
    float ExplosionRadius = 300.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grenade")
    float FuseTime = 1.5f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FX")
    UParticleSystem* GrenadeEffect;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FX")
    USoundBase* GrenadeSound;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FX")
    float GrenadeSoundVolume = 0.5f;

private:
    FTimerHandle FuseTimerHandle;
    bool bHasExploded = false;

    void Explode();

};
