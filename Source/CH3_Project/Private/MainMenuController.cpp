//MainMenuController.h

#include "MainMenuController.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/CameraActor.h"



AMainMenuController::AMainMenuController()
{
	bShowMouseCursor = true;
}


void AMainMenuController::BeginPlay()
{
	Super::BeginPlay();

	SetupMainMenuCamera();

	if (MainMenuWidgetClass)
	{
		MainMenuWidgetInstance = CreateWidget<UUserWidget>(this, MainMenuWidgetClass);
		if (MainMenuWidgetInstance)
		{
			MainMenuWidgetInstance->AddToViewport(999);

			bShowMouseCursor = true;
			SetInputMode(FInputModeUIOnly());
		}
	}
}

void AMainMenuController::SetupMainMenuCamera()
{
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), TEXT("MainMenuCamera"), FoundActors);

	if (FoundActors.Num() > 0)
	{
		ACameraActor* TargetCamera = Cast<ACameraActor>(FoundActors[0]);
		if (TargetCamera)
		{
			SetViewTargetWithBlend(TargetCamera);
		}
	}
}
