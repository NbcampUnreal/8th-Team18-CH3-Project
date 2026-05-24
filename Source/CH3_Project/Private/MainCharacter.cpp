// MainCharacter.cpp

#include "MainCharacter.h"
#include "MainPlayerController.h"
#include "CH3_Project/ShooterGameMode.h"
#include "EnhancedInputComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "TimerManager.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Battle Logic/CombatComponent.h"

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
    NormalCapsuleHalfHeight = 88.0f;
    SlideCapsuleHalfHeight = 40.0f;
    SlideDuration = 1.0f;
    SlideSpeed = 900.0f;
    bIsSliding = false;

    PlayerMaxHP = 100.0f;
    CurrentPlayerHP = PlayerMaxHP;

    MaxAmmo = 100.0f;
    CurrentAmmo = MaxAmmo;

    WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
    WeaponMesh->SetupAttachment(GetMesh());

    CombatComponent = CreateDefaultSubobject<UCombatComponent>(TEXT("CombatComponent"));
}


void AMainCharacter::BeginPlay()
{
    Super::BeginPlay();

    NormalCapsuleHalfHeight = GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight();

    if (WeaponMesh)
    {
        WeaponMesh->AttachToComponent(
            GetMesh(),
            FAttachmentTransformRules::SnapToTargetIncludingScale,
            TEXT("hand_rSocket")
        );

    }
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
    if (bIsSliding)
    {
        DamageAmount = 0.0f;
    }
    float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

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
            if (PlayerController->WeaponAction)
            {
                EnhancedInput->BindAction(
                    PlayerController->WeaponAction,
                    ETriggerEvent::Started,
                    this,
                    &AMainCharacter::FireWeapon
                );
            }
            if (PlayerController->GrenadeAction)
            {
                EnhancedInput->BindAction(
                    PlayerController->GrenadeAction,
                    ETriggerEvent::Started,
                    this,
                    &AMainCharacter::FireGrenade
                );
            }
            if (PlayerController->DanceAction)
            {
                EnhancedInput->BindAction(
                    PlayerController->DanceAction,
                    ETriggerEvent::Started,
                    this,
                    &AMainCharacter::StartDance
                );
            }
        }
    }
}

void AMainCharacter::Move(const FInputActionValue& value)
{
    if (!Controller) return;

    const FVector2D MoveInput = value.Get<FVector2D>();

    if (!MoveInput.IsNearlyZero())
    {
        bIsDancing = false;
    }
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
    if (bIsDancing) return;
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

    if (!bCanSlide || bIsSliding || GetCharacterMovement()->IsFalling()) return;


    if (GetCharacterMovement()->Velocity.Size2D() < 10.0f) return;


    bCanSlide = false;
    bIsSliding = true;


    GetCharacterMovement()->MaxWalkSpeed = SlideSpeed;


    if (GetCapsuleComponent())
    {
        float HeightDifference = NormalCapsuleHalfHeight - SlideCapsuleHalfHeight;
        GetCapsuleComponent()->SetCapsuleHalfHeight(SlideCapsuleHalfHeight);

        if (GetMesh())
        {
            FVector MeshOffset = GetMesh()->GetRelativeLocation();
            MeshOffset.Z += HeightDifference;
            GetMesh()->SetRelativeLocation(MeshOffset);
        }
    }


    FTimerHandle SlideTimerHandle;
    GetWorldTimerManager().SetTimer(SlideTimerHandle, this, &AMainCharacter::StopSlide, SlideDuration, false);


    FTimerHandle CooldownTimerHandle;
    GetWorldTimerManager().SetTimer(CooldownTimerHandle, this, &AMainCharacter::ResetSlideCooldown, 3.0f, false);
}

void AMainCharacter::StopSlide()
{
    if (!bIsSliding) return;

    bIsSliding = false;

    GetCharacterMovement()->MaxWalkSpeed = NormalSpeed;


    if (GetCapsuleComponent())
    {
        float HeightDifference = NormalCapsuleHalfHeight - SlideCapsuleHalfHeight;
        GetCapsuleComponent()->SetCapsuleHalfHeight(NormalCapsuleHalfHeight);

        if (GetMesh())
        {
            FVector MeshOffset = GetMesh()->GetRelativeLocation();
            MeshOffset.Z -= HeightDifference;
            GetMesh()->SetRelativeLocation(MeshOffset);
        }
    }
}


void AMainCharacter::ResetSlideCooldown()
{
    bCanSlide = true;
}

void AMainCharacter::Attack()
{
    if (bIsDancing) return;

    if (FireMontage)
    {
        PlayAnimMontage(FireMontage);
    }
}

void AMainCharacter::StopAttack()
{
    bIsAttacking = false;
}

void AMainCharacter::StartDance()
{
    bIsDancing = !bIsDancing;
}

void AMainCharacter::StopDance()
{
    bIsDancing = false;
}

void AMainCharacter::FireWeapon()
{
    if (CombatComponent)
    {
        CombatComponent->FireWeapon();
    }

    Attack();
}

void AMainCharacter::FireGrenade()
{
    if (bIsThrowingGrenade) return;

    bIsThrowingGrenade = true;

    if (CombatComponent)
    {
        CombatComponent->FireGrenade();
    }

    FTimerHandle GrenadeTimerHandle;

    GetWorldTimerManager().SetTimer(
        GrenadeTimerHandle,
        this,
        &AMainCharacter::StopGrenadeThrow,
        1.0f,
        false
    );
}

void AMainCharacter::StopGrenadeThrow()
{
    bIsThrowingGrenade = false;

}