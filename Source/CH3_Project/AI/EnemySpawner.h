// EnemySpawner.h

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemySpawner.generated.h"

class AEnemyCharacter;

UCLASS()
class CH3_PROJECT_API AEnemySpawner : public AActor
{
    GENERATED_BODY()

public:
    AEnemySpawner();

protected:
    virtual void BeginPlay() override;

public:
    // 스폰할 적 클래스
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy Spawn")
    TSubclassOf<AEnemyCharacter> EnemyClass;

    // 생성할 적의 수
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy Spawn")
    int32 EnemySpawnCount = 10;

    // 적을 배치할 반경
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy Spawn")
    float SpawnRadius = 2000.f;

private:
    // 적 스폰 함수
    void SpawnEnemies();
};
