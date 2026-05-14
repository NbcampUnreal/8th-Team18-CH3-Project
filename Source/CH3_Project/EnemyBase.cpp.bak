// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyBase.h"
#include "ShooterGameMode.h"
#include "Kismet/GameplayStatics.h"

AEnemyBase::AEnemyBase()
{
	HP = 100.f;
	ScoreValue = 1;
}

void AEnemyBase::BeginPlay()
{
	Super::BeginPlay();
}

void AEnemyBase::TakeDamage(float Damage)
{
	HP -= Damage;

	if (HP <= 0)
	{
		AShooterGameMode* GM = Cast<AShooterGameMode>(UGameplayStatics::GetGameMode(this));

		if (GM)
		{
			GM->AddScore(ScoreValue);
		}

		Destroy();
	}
}

