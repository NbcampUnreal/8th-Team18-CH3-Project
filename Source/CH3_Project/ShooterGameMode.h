
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ShooterGameMode.generated.h"


UCLASS()
class CH3_PROJECT_API AShooterGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AShooterGameMode();

protected:
	virtual void BeginPlay() override;

public:

	// 점수
	UPROPERTY(BlueprintReadOnly)
	int32 Score;

	// 목표 점수 (클리어 조건)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 TargetScore;

	// 남은 시간
	UPROPERTY(BlueprintReadOnly)
	float RemainingTime;

	// 제한 시간
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float TimeLimit;

	// 게임 상태
	bool bGameOver;

	// 적 처치 시 호출
	void AddScore(int32 Amount);

	// 게임 종료 처리
	void GameOver(bool bClear);

private:
	void TickTimer();
	FTimerHandle TimerHandle;
	
};
