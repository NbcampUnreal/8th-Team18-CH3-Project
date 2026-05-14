// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemyBase.generated.h"

UCLASS()
class CH3_PROJECT_API AEnemyBase : public AActor
{
	GENERATED_BODY()

public:
	AEnemyBase();

protected:
	virtual void BeginPlay() override;

public:

	// 체력
	UPROPERTY(EditAnywhere)
	float HP;

	// 점수
	UPROPERTY(EditAnywhere)
	int32 ScoreValue;

	// 데미지 받기
	void TakeDamage(float Damage);

};
