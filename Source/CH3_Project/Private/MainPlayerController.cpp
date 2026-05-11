//MainPlayerController.cpp

#include "MainPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "UI_Widget.h"

AMainPlayerController::AMainPlayerController()
    :InputMappingContext(nullptr),
    MoveAction(nullptr),
    JumpAction(nullptr),
    LookAction(nullptr),
    SprintAction(nullptr),
    UUI_WidgetClass(nullptr)
{

}


void AMainPlayerController::BeginPlay()
{
    Super::BeginPlay();

    
    if (ULocalPlayer* LocalPlayer = GetLocalPlayer())
    {
        
        if (UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
        {
            if (InputMappingContext)
            {
                
                Subsystem->AddMappingContext(InputMappingContext, 0);
            }
        }
    }

}

void AMainPlayerController::OnPossess(APawn* aPawn)
{
    Super::OnPossess(aPawn);
    {
        if (UUI_WidgetClass)
        {
            UUI_WidgetInstance = CreateWidget<UUI_Widget>(this, UUI_WidgetClass);
            if (UUI_WidgetInstance)
            {
                UUI_WidgetInstance->AddToViewport();
            }
        }

    }
}
