// Fill out your copyright notice in the Description page of Project Settings.

#include "ShooterGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"

AShooterGameMode::AShooterGameMode()
{
	Score = 0;
	TargetScore = 10;
	TimeLimit = 60.f;
	RemainingTime = TimeLimit;
	bGameOver = false;
}

void AShooterGameMode::BeginPlay()
{
	Super::BeginPlay();

	RemainingTime = TimeLimit;

	// 1초마다 타이머 실행
	GetWorldTimerManager().SetTimer(TimerHandle, this, &AShooterGameMode::TickTimer, 1.0f, true);
}

void AShooterGameMode::TickTimer()
{
	if (bGameOver) return;

	RemainingTime--;

	// 시간 초과 → 실패
	if (RemainingTime <= 0)
	{
		GameOver(false);
	}
}

void AShooterGameMode::AddScore(int32 Amount)
{
	if (bGameOver) return;

	Score += Amount;

	UE_LOG(LogTemp, Warning, TEXT("Score: %d"), Score);

	// 클리어 조건
	if (Score >= TargetScore)
	{
		GameOver(true);
	}
}

void AShooterGameMode::GameOver(bool bClear)
{
	if (bGameOver) return;

	bGameOver = true;

	GetWorldTimerManager().ClearTimer(TimerHandle);

	if (bClear)
	{
		UE_LOG(LogTemp, Warning, TEXT("GAME CLEAR"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("GAME OVER"));
	}

	// 입력 막기
	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(this, 0);
	if (PlayerPawn)
	{
		PlayerPawn->DisableInput(nullptr);
	}
}