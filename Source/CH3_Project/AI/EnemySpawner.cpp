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

// 적 스폰 함수
void AEnemySpawner::SpawnEnemies()
{
    // 스폰할 적 클래스가 지정되지 않았으면 아무것도 하지 않음
    if (!EnemyClass)
    {
        return;
    }

    // 현재 월드의 Navigation System을 가져옴
    UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetCurrent(GetWorld());
    if (!NavSystem)
    {
        return;
    }

    // 스포너 액터의 위치를 랜덤 스폰 중심으로 사용
    const FVector Center = GetActorLocation();

    // 스폰에 성공한 적 수
    int32 SpawnedCount = 0;
    // 스폰 시도 횟수
    int32 TryCount = 0;

    // 무한루프 방지를 위한 최대 시도 횟수
    // ex) 적 10명 스폰시 최대 100번까지 위치를 찾아봄
    const int32 MaxTryCount = EnemySpawnCount * 10;

    // 목표 스폰 수를 채우거나 최대 시도 횟수에 도달할때 까지 반복
    while (SpawnedCount < EnemySpawnCount && TryCount < MaxTryCount)
    {
        TryCount++;

        // NavMesh 위에서 찾은 랜덤 위치를 담을 변수
        FNavLocation RandomLocation;

        // 스포너 위치 센터 기준 SpawnRadius 안에서 적 캐릭터가 스폰 가능한 위치를 랜덤으로 찾음
        const bool bFoundLocation = NavSystem->GetRandomReachablePointInRadius(
            Center,
            SpawnRadius,
            RandomLocation
        );

        // 적절한 위치를 못 찾으면 건너뜀
        if (!bFoundLocation)
        {
            continue;
        }

        // 스폰 옵션 설정
        FActorSpawnParameters SpawnParams;
        SpawnParams.SpawnCollisionHandlingOverride =
            ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

        // EnemyClass로 지정된 적을 랜덤 위치에 생성
        AEnemyCharacter* SpawnedEnemy = GetWorld()->SpawnActor<AEnemyCharacter>(
            EnemyClass,
            RandomLocation.Location,
            FRotator::ZeroRotator,
            SpawnParams
        );

        // 스폰에 성공했을때 카운트 증가
        if (SpawnedEnemy)
        {
            SpawnedCount++;
        }
    }
}