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
 	
	PrimaryActorTick.bCanEverTick = false;


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
    if (!Controller) return;

    const FVector2D MoveInput = value.Get<FVector2D>();

    if (!FMath::IsNearlyZero(MoveInput.X))
    {
        AddMovementInput(GetActorForwardVector(), MoveInput.X);
    }

    if (!FMath::IsNearlyZero(MoveInput.Y))
    {
        AddMovementInput(GetActorRightVector(), MoveInput.Y);
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
    if (CurrentPlayerHP > PlayerMaxHP * 0.5f)
    {
        GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
    }
}

void AMainCharacter::StopSprint(const FInputActionValue& value)
{
    if (GetCharacterMovement())
    {
        GetCharacterMovement()->MaxWalkSpeed = NormalSpeed;
    }
}

void AMainCharacter::StartSlide(const FInputActionValue& value)
{
    if (bIsSliding)
    {
        return;
    }

    if (!GetCharacterMovement())
    {
        return;
    }

    
    if (CurrentPlayerHP <= PlayerMaxHP * 0.5f)
    {
        return;
    }

    
    if (GetCharacterMovement()->MaxWalkSpeed < SprintSpeed)
    {
        return;
    }

    bIsSliding = true;


    GetCapsuleComponent()->SetCapsuleHalfHeight(SlideCapsuleHalfHeight);

    
    GetCharacterMovement()->MaxWalkSpeed = SlideSpeed;

    
    SpringArmComp->TargetOffset.Z = -40.0f;

    
    LaunchCharacter(GetActorForwardVector() * 600.0f, true, true);

 
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

    
    GetCapsuleComponent()->SetCapsuleHalfHeight(NormalCapsuleHalfHeight);

    
    GetCharacterMovement()->MaxWalkSpeed = NormalSpeed;

   
    SpringArmComp->TargetOffset.Z = 0.0f;
}
