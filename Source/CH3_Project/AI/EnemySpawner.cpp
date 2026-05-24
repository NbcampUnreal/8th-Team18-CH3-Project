// EnemySpawner.cpp

#include "EnemySpawner.h"
#include "EnemyCharacter.h"
#include "NavigationSystem.h"

AEnemySpawner::AEnemySpawner()
{
    PrimaryActorTick.bCanEverTick = false;
}

void AEnemySpawner::BeginPlay()
{
    Super::BeginPlay();

    SpawnEnemies();
}

void AEnemySpawner::SpawnEnemies()
{
    if (!EnemyClass)
    {
        return;
    }

    UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetCurrent(GetWorld());
    if (!NavSystem)
    {
        return;
    }

    const FVector Center = GetActorLocation();

    int32 SpawnedCount = 0;
    int32 TryCount = 0;
    const int32 MaxTryCount = EnemySpawnCount * 10;

    while (SpawnedCount < EnemySpawnCount && TryCount < MaxTryCount)
    {
        TryCount++;

        FNavLocation RandomLocation;

        const bool bFoundLocation = NavSystem->GetRandomReachablePointInRadius(
            Center,
            SpawnRadius,
            RandomLocation
        );

        if (!bFoundLocation)
        {
            continue;
        }

        FActorSpawnParameters SpawnParams;
        SpawnParams.SpawnCollisionHandlingOverride =
            ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

        AEnemyCharacter* SpawnedEnemy = GetWorld()->SpawnActor<AEnemyCharacter>(
            EnemyClass,
            RandomLocation.Location,
            FRotator::ZeroRotator,
            SpawnParams
        );

        if (SpawnedEnemy)
        {
            SpawnedCount++;
        }
    }
}