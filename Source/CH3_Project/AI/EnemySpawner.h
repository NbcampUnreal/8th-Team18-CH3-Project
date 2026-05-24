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
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy Spawn")
    TSubclassOf<AEnemyCharacter> EnemyClass;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy Spawn")
    int32 EnemySpawnCount = 10;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy Spawn")
    float SpawnRadius = 2000.f;

private:
    void SpawnEnemies();
};
