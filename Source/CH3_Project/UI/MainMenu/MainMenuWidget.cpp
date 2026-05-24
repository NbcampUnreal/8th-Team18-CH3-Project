//MainMenuWidget.cpp

#include "UI/MainMenu/MainMenuWidget.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include "CH3_Project.h"

void UMainMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (StartButton)
	{
		MYLOG("StartButton On");
		StartButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OnStartButtonClicked);

	}
	else
	{
		MYLOG("StartButton off");
	}
}


void UMainMenuWidget::OnStartButtonClicked()
{
	APlayerCameraManager* CameraManager = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0);
	if (CameraManager)
	{
		CameraManager->StartCameraFade(0.f, 1.f, 1.5f, FLinearColor::Black, false, true);
	}

	if (StartButton)
	{
		StartButton->SetIsEnabled(false);
	}

	FTimerHandle LevelLoadTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(
		LevelLoadTimerHandle,
		FTimerDelegate::CreateLambda([this]()
			{
				UGameplayStatics::OpenLevel(GetWorld(), FName("PlayerGround"));
			}),
		1.5f,
		false
		);




}
