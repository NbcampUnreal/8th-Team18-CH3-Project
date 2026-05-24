// MainCharacter.h

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "TimerManager.h"
#include "GameFramework/Character.h"
#include "Animation/AnimMontage.h"
#include "Components/ActorComponent.h"
#include "MainCharacter.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FHealthDeadSignature, APlayerController*, Instigator);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FEnemyKilledSignature, APlayerController*, Instigator);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FHealthDamagedSignature, float, UpdatePlayerHP, float, PlayerMaxHP, float, CurrentPlayerHP);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FMissionUpdatedSignature, FString, CurrentMissionName, int32, MissionCurrentScore, int32, MissionMaxScore);

class USpringArmComponent;
class UCameraComponent;
class UCombatComponent;
struct FInputActionValue;

UCLASS()
class CH3_PROJECT_API AMainCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AMainCharacter();


	UPROPERTY(BlueprintAssignable, Category = "Status")
	FHealthDamagedSignature HealthDamaged;

	UFUNCTION(BlueprintPure, Category = "Status")
	float GetHealth() const;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")

	float PlayerMaxHP;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")

	float CurrentPlayerHP;
	UFUNCTION(BlueprintPure, Category = "Weapon")

	int32 GetMaxAmmo() const;

	UFUNCTION(BlueprintPure, Category = "Weapon")
	int32 GetCurrentAmmo() const;

	UPROPERTY(BlueprintAssignable, Category = "Mission")
	FMissionUpdatedSignature MissionUpdateBoard;

	UFUNCTION(BlueprintCallable, Category = "Mission")
	void GetMissionProgress(int32 Amount);

	UPROPERTY(BlueprintAssignable, Category = "EnemyKill")
	FEnemyKilledSignature EnemyKillUpdate;

	UFUNCTION(BlueprintCallable, Category = "EnemyKill")
	void EnemyKilledSignature(APlayerController* InstigatorController) const;

	UFUNCTION(BlueprintCallable, Category = "EnemyKill")
	void EnemyKillAnimation();

	UPROPERTY(BlueprintReadOnly, Category = "Slide")
	bool bIsSliding = false;


protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	USpringArmComponent* SpringArmComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	UCameraComponent* CameraComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	int32 MaxAmmo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	int32 CurrentAmmo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float NormalSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float SprintSpeedMultiplier;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	float SprintSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Slide")
	float SlideCapsuleHalfHeight = 40.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Slide")
	float NormalCapsuleHalfHeight = 88.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Slide")
	float SlideDuration = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Slide")
	float SlideSpeed = 900.0f;


	bool bCanSlide = true;

	void ResetSlideCooldown();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mission")
	FString CurrentMissionName = TEXT("Mission Score");

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mission")
	int32 MissionCurrentScore = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mission")

	int32 MissionMaxScore = 1000;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
	USkeletalMeshComponent* WeaponMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
	UCombatComponent* CombatComponent;

	UFUNCTION()
	void FireWeapon();

	UFUNCTION()
	void FireGrenade();

	UPROPERTY(BlueprintReadOnly, Category = "Combat")
	bool bIsAttacking = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	UAnimMontage* FireMontage;

	UFUNCTION()
	void Attack();

	UPROPERTY(BlueprintReadOnly, Category = "Combat")
	bool bIsThrowingGrenade = false;

	void StopGrenadeThrow();

	void StopAttack();

	UPROPERTY(BlueprintReadOnly, Category = "Dance")
	bool bIsDancing = false;

	UFUNCTION()
	void StartDance();

	UFUNCTION()
	void StopDance();

	UFUNCTION()
	void Move(const FInputActionValue& value);

	UFUNCTION()

	void StartJump(const FInputActionValue& value);

	UFUNCTION()

	void StopJump(const FInputActionValue& value);

	UFUNCTION()

	void Look(const FInputActionValue& value);

	UFUNCTION()

	void StartSprint(const FInputActionValue& value);

	UFUNCTION()

	void StopSprint(const FInputActionValue& value);

	UFUNCTION()

	void StartSlide(const FInputActionValue& value);

	UFUNCTION()

	void StopSlide();




	virtual void BeginPlay() override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual float TakeDamage(

		float DamageAmount,

		struct FDamageEvent const& DamageEvent,

		AController* EventInstigator,

		AActor* DamageCauser) override;
};