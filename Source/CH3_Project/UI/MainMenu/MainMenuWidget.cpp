//MainMenuWidget.cpp

#include "UI/MainMenu/MainMenuWidget.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
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
	UGameplayStatics::OpenLevel(GetWorld(), FName("TestMap"));
}
