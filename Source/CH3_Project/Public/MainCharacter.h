
#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "TimerManager.h"
#include "GameFramework/Character.h"
#include "MainCharacter.generated.h"


class USpringArmComponent;
class UCameraComponent;

struct FInputActionValue;

UCLASS()
class CH3_PROJECT_API AMainCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AMainCharacter();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	USpringArmComponent* SpringArmComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	UCameraComponent* CameraComp;

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
	UPROPERTY(BlueprintReadOnly, Category = "Slide")
	bool bIsSliding = false;

	FTimerHandle SlideTimerHandle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
	float PlayerMaxHP = 100.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
	float CurrentPlayerHP = 100.f;

	UFUNCTION(BlueprintPure, Category = "Status")
	float GetHealth() const;



	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual float TakeDamage(
		float DamageAmount,
		struct FDamageEvent const& DamageEvent,
		AController* EventInstigator,
		AActor* DamageCauser) override;

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
};
