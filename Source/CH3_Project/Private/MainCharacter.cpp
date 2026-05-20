//MainCharacter.cpp


#include "MainCharacter.h"
#include "MainPlayerController.h"
#include "CH3_Project/ShooterGameMode.h"
#include "EnhancedInputComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "TimerManager.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"



AMainCharacter::AMainCharacter()
{
 	
	PrimaryActorTick.bCanEverTick = true;


	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArmComp->SetupAttachment(RootComponent);
	SpringArmComp->TargetArmLength = 300.0f;
	SpringArmComp->bUsePawnControlRotation = true;

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComp->SetupAttachment(SpringArmComp, USpringArmComponent::SocketName);
	CameraComp->bUsePawnControlRotation = false;

    NormalSpeed = 600.0f;
    SprintSpeedMultiplier = 1.5f;
    SprintSpeed = NormalSpeed * SprintSpeedMultiplier;

    GetCharacterMovement()->MaxWalkSpeed = NormalSpeed;

    GetCharacterMovement()->BrakingDecelerationWalking = 2048.0f;
    GetCharacterMovement()->GroundFriction = 8.0f;
    GetCharacterMovement()->BrakingFrictionFactor = 2.0f;

    NormalCapsuleHalfHeight =
        GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight();

    PlayerMaxHP = 100.0f;
    CurrentPlayerHP = PlayerMaxHP;

    MaxAmmo = 100.0f;
    CurrentAmmo = MaxAmmo;
}


void AMainCharacter::BeginPlay()
{
    Super::BeginPlay();
    GetCharacterMovement()->bMaintainHorizontalGroundVelocity = true;
    
}


float AMainCharacter::GetHealth() const
{
    if (PlayerMaxHP <= 0.0f) return 0.0f;
    return CurrentPlayerHP / PlayerMaxHP;
}


int32 AMainCharacter::GetMaxAmmo() const
{
    return MaxAmmo;
}

int32 AMainCharacter::GetCurrentAmmo() const
{
    return CurrentAmmo;
}

void AMainCharacter::EnemyKilledSignature(APlayerController* InstigatorController) const
{
    EnemyKillUpdate.Broadcast(InstigatorController);
}

void AMainCharacter::EnemyKillAnimation()
{
}

void AMainCharacter::GetMissionProgress(int32 Amount)
{
    MissionCurrentScore += Amount;
    MissionCurrentScore = FMath::Clamp(MissionCurrentScore, 0, MissionMaxScore);
    MissionUpdateBoard.Broadcast(CurrentMissionName, MissionCurrentScore, MissionMaxScore);
}


float AMainCharacter::TakeDamage(float DamageAmount, 
    FDamageEvent const& DamageEvent,
    AController* EventInstigator, 
    AActor* DamageCauser)
{
   float ActualDamage =  Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

   CurrentPlayerHP = FMath::Clamp(CurrentPlayerHP - DamageAmount, 0.0f, PlayerMaxHP);
   HealthDamaged.Broadcast(CurrentPlayerHP, PlayerMaxHP, DamageAmount);

   if (CurrentPlayerHP <= PlayerMaxHP * 0.5f)
   {
       GetCharacterMovement()->MaxWalkSpeed = NormalSpeed;
   }
   if (CurrentPlayerHP <= 0.0f)
   {
       void GameOver();
   }

   return ActualDamage;
}


void AMainCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
 
    if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
    {
        if (AMainPlayerController* PlayerController = Cast<AMainPlayerController>(GetController()))
        {
            if (PlayerController->MoveAction)
            {
                
                EnhancedInput->BindAction(
                    PlayerController->MoveAction,
                    ETriggerEvent::Triggered,
                    this,
                    &AMainCharacter::Move
                );
            }

            if (PlayerController->JumpAction)
            {
                EnhancedInput->BindAction(
                    PlayerController->JumpAction,
                    ETriggerEvent::Triggered,
                    this,
                    &AMainCharacter::StartJump
                );

                EnhancedInput->BindAction(
                    PlayerController->JumpAction,
                    ETriggerEvent::Completed,
                    this,
                    &AMainCharacter::StopJump
                );
            }

            if (PlayerController->LookAction)
            {
               
                EnhancedInput->BindAction(
                    PlayerController->LookAction,
                    ETriggerEvent::Triggered,
                    this,
                    &AMainCharacter::Look
                );
            }

            if (PlayerController->SprintAction)
            {
                EnhancedInput->BindAction(
                    PlayerController->SprintAction,
                    ETriggerEvent::Triggered,
                    this,
                    &AMainCharacter::StartSprint
                ); 
                EnhancedInput->BindAction(
                    PlayerController->SprintAction,
                    ETriggerEvent::Completed,
                    this,
                    &AMainCharacter::StopSprint
                );
            }
            if (PlayerController->SlideAction)
            {
                EnhancedInput->BindAction(
                    PlayerController->SlideAction,
                    ETriggerEvent::Started,
                    this,
                    &AMainCharacter::StartSlide
                );
            }
        }
    }
}



void AMainCharacter::Move(const FInputActionValue& value)
{
    if (!Controller)
    {
        return;
    }

    const FVector2D MoveInput = value.Get<FVector2D>();

    const FRotator ControlRot = Controller->GetControlRotation();
    const FRotator YawRot(0.f, ControlRot.Yaw, 0.f);

    const FVector Forward = FRotationMatrix(YawRot).GetUnitAxis(EAxis::X);
    const FVector Right = FRotationMatrix(YawRot).GetUnitAxis(EAxis::Y);

    if (!FMath::IsNearlyZero(MoveInput.X))
    {
        AddMovementInput(Forward, MoveInput.X);
    }

    if (!FMath::IsNearlyZero(MoveInput.Y))
    {
        AddMovementInput(Right, MoveInput.Y);
    }
}

void AMainCharacter::StartJump(const FInputActionValue& value)
{
    if (value.Get<bool>())
    {
        Jump();
    }
}

void AMainCharacter::StopJump(const FInputActionValue& value)
{
    if (!value.Get<bool>())
    {
        StopJumping();
    }
}

void AMainCharacter::Look(const FInputActionValue& value)
{
    FVector2D LookInput = value.Get<FVector2D>();

    AddControllerYawInput(LookInput.X);
    AddControllerPitchInput(LookInput.Y);
}

void AMainCharacter::StartSprint(const FInputActionValue& value)
{
    if (!GetCharacterMovement())
    {
        return;
    }
    if (!bIsSliding && CurrentPlayerHP > PlayerMaxHP * 0.5f)
    {
        GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
    }
}

void AMainCharacter::StopSprint(const FInputActionValue& value)
{
    if (!GetCharacterMovement()) return;

    bIsSprinting = false; 

    if (!bIsSliding)
    {
        GetCharacterMovement()->MaxWalkSpeed = NormalSpeed;
    }
}

void AMainCharacter::StartSlide(const FInputActionValue& value)
{
    // 쿨타임 및 슬라이드 중 체크 (C++에서 확실히 차단)
    if (bIsSliding || !bCanSlide)
    {
        return;
    }

    bIsSliding = true;
    bCanSlide = false;

    if (GetCharacterMovement())
    {
        GetCharacterMovement()->MaxWalkSpeed = SlideSpeed; // 속도 증가 (900.0f)
    }

    // 엔진 내장 크라우치 기능 사용 (바닥 뚫림을 엔진이 자동으로 방지해 줌)
    Crouch();

    GetWorldTimerManager().SetTimer(
        SlideTimerHandle,
        this,
        &AMainCharacter::StopSlide,
        SlideDuration,
        false
    );
}

void AMainCharacter::StopSlide()
{
    bIsSliding = false;

    // 엔진 내장 크라우치 해제
    UnCrouch();

    if (GetCharacterMovement())
    {
        if (bIsSprinting && CurrentPlayerHP > PlayerMaxHP * 0.5f)
        {
            GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
        }
        else
        {
            GetCharacterMovement()->MaxWalkSpeed = NormalSpeed;
        }
    }

    // 쿨타임 타이머 시작
    GetWorldTimerManager().SetTimer(
        SlideCooldownHandle,
        this,
        &AMainCharacter::ResetSlideCooldown,
        SlideCooldown,
        false
    );
}

void AMainCharacter::ResetSlideCooldown()
{
    bCanSlide = true;
}

